/*
 * backup.cc
 *
 * rpc server for backup server
 *
 */

#include "backup.h"
#include <fstream>
#include "rpc/server.h"
#include <iostream>

BackupServerBackEnd::BackupServerBackEnd(int port_num) :
		port_num(port_num), running_(true), commiting_logs_thread_(
				GetCommitingLogsThread()), check_view_change_thread_(
				GetViewChangeThread()) {
}

BackupServerBackEnd::~BackupServerBackEnd() {
	running_ = false;
	commiting_logs_thread_.join();
	check_view_change_thread_.join();
}

/*
 * Start the backup server
 *
 * fetch the logs from the witness server,
 * Inform the Primary that the backup server is alive.
 */
void BackupServerBackEnd::Start() {
	if (!witness_server_) {
		std::cout << "Should call SetWitnessServer() first. \n";
	}
	PayLoad payload = witness_server_->GetLogRecords();
	for (const auto &log : payload.log_record_vector) {
		log_record_list_.push_back(log);
		next_available_log_id_ = log.log_id + 1;
	}
	view_number_ = payload.view_number;
	std::cout << "Backup: next_available_log_id is " << next_available_log_id_
			<< ". \n";

	//check the primary is ok.
	if (primary_server_->BringUpBackUp()) {
		last_request_time_ = GetCurrentTimestamp();
	}

}

long BackupServerBackEnd::GetPromiseTime() {
	promised_time_ = GetCurrentTimestamp();
	std::cout << "backup returns promised_time " << promised_time_ << ". \n";
	return promised_time_;
}

/*
 * Handle the payload from primary server
 *
 * If the payload has empty log, it's heartbeat msg.
 * Otherwise the function check if the log id match the next available log id,
 * and answer the primary it can commit(phase 1 of 2pc).
 *
 * @param payload
 * @return true if received a heartbeat or received some valid log.
 */
bool BackupServerBackEnd::RequestCommit(const PayLoad &payload) {
	last_request_time_ = GetCurrentTimestamp();

	//used as hearbeat
	if (payload.log_record_vector.empty()) {
		std::cout << "Backup server: Received a heartbeat from primary. \n";
		return true;
	}

	//can commit check
	for (const LogRecord &log : payload.log_record_vector) {
		//only accepts log in log order.
		if (log.log_id != next_available_log_id_) {
			return false;
		}

		InsertRecordLog(log);
		std::cout << "file name is " << log.file_name << ". \n";
		std::cout << "file content is " << log.operation_content << ". \n";
		//next_available_log_id_ = log.log_id + 1;
	}
	return true;
}

/*
 * Commit the log
 *
 *
 * Phase 2 of 2pc, Update the commit_point_ to largest
 * commit_point_ it recevied from the primary server.
 * Register the primary server ap pointer.
 *
 * @Param payload
 *
 *
 */
void BackupServerBackEnd::Commit(const PayLoad &payload) {
	commit_point_mtx_.lock();
	if (payload.commit_point > commit_point_) {
		//backup maintains a CP stores the largest CP it has received in message.
		commit_point_ = payload.commit_point;
		server_log_ap = payload.log_ap;
	}
	commit_point_mtx_.unlock();
}

/*
 * Insert a log to the local memory, update top pointer.
 *
 * @Param log_record contains one event.
 */
void BackupServerBackEnd::InsertRecordLog(const LogRecord &log_record) {
	log_record_list_mtx_.lock();
	log_record_list_.push_back(log_record);
	next_available_log_id_ += 1;
	log_record_list_mtx_.unlock();
}

/*
 * Apply the logs
 *
 * execute the events, update the ap pointer.
 */
void BackupServerBackEnd::ApplyLogs() {
	while (running_) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		log_record_list_mtx_.lock();
		while (!log_record_list_.empty()) { //TODO double check this

			if ((!is_primary_) && (log_ap > server_log_ap)) {
				break;
			}

			const LogRecord &log = log_record_list_.front();
			std::cout << "Backup server: Apply Log " << log.log_id << std::endl;
//			commit_point_mtx_.lock();
//			if (commit_point_ < log.log_id) {
//				commit_point_mtx_.unlock();
//				break;
//			}
//			commit_point_mtx_.unlock();

			if (log.operation_name.compare("WriteFile") == 0) {
				std::ofstream file(log.file_name);
				file << log.operation_content;
				file.close();
			}
			log_ap = log.log_id;
			log_record_list_.pop_front();

		}
		log_record_list_mtx_.unlock();
	}
}

/*
 * Recover the backup server to join the group
 *
 * After fetched the logs from witness,
 * if the backup haven't received heartbeats for T seconds,
 * it will set itself as coordinator and tell the witness.
 * This is a simplified bully election algorithm.
 *
 */
std::thread BackupServerBackEnd::GetViewChangeThread() {
	return std::thread([=] {
		//Leave 5 seconds to register the service.
		std::this_thread::sleep_for(std::chrono::seconds(5));
		Start();
		while (running_) {
			std::this_thread::sleep_for (std::chrono::seconds(1));
			if (!is_primary_ && GetCurrentTimestamp() - last_request_time_ > 20) {
				PayLoad payload;
				payload.is_primary_server = false;
				view_number_++;
				payload.view_number = view_number_;
				if (witness_server_->RequestViewChange(payload)) {
					std::cout << "Backup server: Becomes coordinator. \n";
					is_primary_ = true;
					//
				}
				else {
					// Backup gets partitioned.
					ShutDown();
				}
			}
		}
	});
}

/*
 * Only backup server in the group, stop running.
 */
void BackupServerBackEnd::ShutDown() {
	running_ = false;
}

/*
 * Start a thread to apply the logs.
 */
std::thread BackupServerBackEnd::GetCommitingLogsThread() {
	return std::thread([=] {ApplyLogs();});
}

/*
 * Introduce the witness server of the group to the backup server.
 *
 * @param witness_server: rpc client side for the witness server.
 */
void BackupServerBackEnd::SetWitnessServer(
		WitnessServerFrontEnd *witness_server) {
	witness_server_ = witness_server;
}

/*
 * Introduce the primary server of the group to the backup server.
 *
 * @param primary_server: rpc client side for the primary server.
 */
void BackupServerBackEnd::SetPrimaryServer(
		PrimaryServerFrontEnd *primary_server) {
	primary_server_ = primary_server;
}

/*
 * Backup server demote from being coordinator.
 */
void BackupServerBackEnd::Demote() {
	std::cout << "Backup server: Demote from a coordinator." << std::endl;
	is_primary_ = false;
}

/*
 * Client read API
 *
 * when backup server becomes the coordinator, this will be used.
 *
 * @param file_name: file name to be read.
 */
std::string BackupServerBackEnd::ReadFile(const std::string &file_name) {
	if (!is_primary_) {
		std::cout << "Error: Client should call the primary server. \n";
		return "";
	}
	std::ifstream file(file_name);
	std::string file_content, temp;
	while (std::getline(file, temp)) {
		file_content += temp;
	}
	file.close();
	return file_content;
}

/*
 * Client Write API
 *
 * when backup server becomes the coordinator, this will be used.
 *
 * @param file_name: file name to be written to
 * @param file_content: file content
 */
bool BackupServerBackEnd::WriteFile(const std::string &file_name,
		const std::string &file_content) {
	if (!is_primary_) {
		std::cout << "Error. should call primary server. \n";
		return false;
	}
	LogRecord log_record(GetCurrentTimestamp(), next_available_log_id_,
			"WriteFile", file_name, file_content, "backup");
	InsertRecordLog(log_record);

	//The only reason the backup becomes primary is that
	//witness is helping him.
	PayLoad payload( { log_record }, commit_point_, log_ap);
	if (witness_server_->RecordLogRecords(payload) == false)
		return false;

	commit_point_mtx_.lock();
	commit_point_++;
	commit_point_mtx_.unlock();
	return true;
}

int main() {
	//setup the group with known host_ips and port_nums.
	//server will be initialized in the thread
	BackupServerBackEnd backend(8070);
	WitnessServerFrontEnd witnessFrontEnd("127.0.0.1", 8071);
	PrimaryServerFrontEnd primaryFrontEnd("127.0.0.1", 8072);
	backend.SetWitnessServer(&witnessFrontEnd);
	backend.SetPrimaryServer(&primaryFrontEnd);

	//register the service
	rpc::server srv(backend.getPortNum());

	srv.bind("RequestCommit", [&backend](PayLoad payload) {
		return backend.RequestCommit(payload);
	});

	srv.bind("GetPromiseTime", [&backend]() {
		return backend.GetPromiseTime();
	});

	srv.bind("Commit", [&backend](PayLoad payload) {
		return backend.Commit(payload);
	});

	srv.bind("Demote", [&backend]() {
		backend.Demote();
	});

	srv.bind("WriteFile",
			[&backend](std::string file_name, std::string file_content) {
				return backend.WriteFile(file_name, file_content);
			});

	srv.bind("ReadFile", [&backend](std::string file_name) {
		return backend.ReadFile(file_name);
	});

	srv.suppress_exceptions(true); //turn the exception to error response
	std::cout << "Backup server is running... \n";
	srv.run();

	return 0;
}
