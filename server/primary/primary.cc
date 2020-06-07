/*
 * primary.cc
 *
 * rpc server for primary server
 *
 */

#include "primary.h"
#include <chrono>
#include "rpc/server.h"
#include <iostream>
#include <fstream>
#include <string>

PrimaryServerBackEnd::PrimaryServerBackEnd(int port_num) :
		port_num(port_num), running_(true), commiting_logs_thread_(
				GetCommitingLogsThread()), heart_beat_thread_(
				GetHeartBeatThread()) {
	//register myself to the member list
	Member t = { 1, "127.0.0.1", port_num, 1, 0 };
	memberList.push_back(t);
}

PrimaryServerBackEnd::~PrimaryServerBackEnd() {
	running_ = false;
	commiting_logs_thread_.join();
	heart_beat_thread_.join();
}

/*
 * Start the primary server
 *
 * recover the operation log from witness.
 * sends the coordinator msg to the backup server,
 * so the primary server becomes coordinator.
 */
void PrimaryServerBackEnd::Start() {
	if (!witness_server_) {
		std::cout << "Should call SetWitnessServer() first. \n";
	}
	PayLoad payload = witness_server_->GetLogRecords();
	for (const auto &log : payload.log_record_vector) {
		log_record_list_.push_back(log);
		next_available_log_id_ = log.log_id + 1;
	}
	view_number_ = payload.view_number;
	std::cout << "Primary: next_available_log_id is " << next_available_log_id_
			<< ". \n";
	backup_server_frontend_->Demote();
	std::cout << "Primary started successfully\n";
}

/*
 * Client API WriteFile
 *
 * check back up servers can commit
 * do asynchronous commit,
 * return the result to client.
 *
 * @param file_name: file to write
 * @param file_content: file content
 * @return true: write committed.
 */
bool PrimaryServerBackEnd::WriteFile(const std::string &file_name,
		const std::string &file_content) {
	std::cout << "FrontEnd calls WriteFile() " << file_name << ". \n";
	LogRecord log_record(GetCurrentTimestamp(), next_available_log_id_++,
			"WriteFile", file_name, file_content, "primary");
	InsertRecordLog(log_record);

	//Check backup servers can commit.
	PayLoad payload( { log_record }, commit_point_, log_ap);
	if (!is_backup_down_) {
		if (backup_server_frontend_->RequestCommit(payload) == false) {
			return false;
		}
	} else {
		if (witness_server_->RecordLogRecords(payload) == false)
			return false;
	}

	//Do asynchronous commit
	cp_mutex.lock();
	commit_point_++;

	PayLoad payload2( { }, commit_point_, log_ap);

	if (!is_backup_down_) {
		backup_server_frontend_->Commit(payload2);
	}
	//witness_server -> Commit(payload);

	cp_mutex.unlock();

	//send commit to backups asynchronously.

	return true;
}

/*
 * Client API ReadFile
 *
 * read from local server
 *
 * @param file_name: file to read
 * @return file_content: file content.
 */
std::string PrimaryServerBackEnd::ReadFile(const std::string &file_name) {
	if (promised_time_ < GetCurrentTimestamp()) {
		if (!is_backup_down_) {
			promised_time_ = backup_server_frontend_->GetPromiseTime();
		}
	}
	std::ifstream file(file_name);
	std::string file_content, temp;
	while (std::getline(file, temp)) {
		file_content += temp;
	}

	file.close();

	std::cout << file_content << std::endl;
	return file_content;
}

/*
 * Add an operation to primary server log.
 */
void PrimaryServerBackEnd::InsertRecordLog(const LogRecord &log_record) {
	log_record_list_mtx_.lock();
	log_record_list_.push_back(log_record);
	last_request_time_ = GetCurrentTimestamp();
	log_record_list_mtx_.unlock();
}

/*
 * Thread to apply the event records.
 */
void PrimaryServerBackEnd::ApplyLogs() {
	while (running_) {
		//mimic some delay for processing
		std::this_thread::sleep_for(std::chrono::seconds(1));
		log_record_list_mtx_.lock();
		//int num_committed = 0;
		while (!log_record_list_.empty()) {
			//num_committed++;

			const LogRecord &log = log_record_list_.front();
			//apply the write
			if (log.operation_name.compare("WriteFile") == 0) {
				std::ofstream file(log.file_name);
				file << log.operation_content;
				file.close();
			}
			//update apply pointer.
			log_ap = log.log_id;
			log_record_list_.pop_front();
		}
		log_record_list_mtx_.unlock();
//		if (!is_backup_down_ && num_committed > 0) {
//			backup_server_frontend_->Commit( { { }, commit_point_ });
//		}
	}
}

std::thread PrimaryServerBackEnd::GetCommitingLogsThread() {
	return std::thread([=] {ApplyLogs();});
}

void PrimaryServerBackEnd::GetHeartBeat() {
	// leave 5 seconds to finish register the service.
	std::this_thread::sleep_for(std::chrono::seconds(5));
	Start();
	while (running_ && !no_response_) {
		// Emits heartbeat at least every 2 seconds.
		if (GetCurrentTimestamp() - last_request_time_ > 2) {
			PayLoad payload;
			if (is_backup_down_) {
				if (witness_server_->RecordLogRecords(payload)) {
					log_record_list_mtx_.lock();
					last_request_time_ = GetCurrentTimestamp();
					log_record_list_mtx_.unlock();
				}
			} else {
				if (backup_server_frontend_->RequestCommit(payload) == true) {
					log_record_list_mtx_.lock();
					last_request_time_ = GetCurrentTimestamp();
					log_record_list_mtx_.unlock();
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));

		if (GetCurrentTimestamp() - last_request_time_ > 10) {
			PayLoad payload;
			payload.is_primary_server = true;
			view_number_++;
			payload.view_number = view_number_;
			if (witness_server_->RequestViewChange(payload)) {
				std::cout << "new view number is " << view_number_ << ". \n";
				last_request_time_ = GetCurrentTimestamp();
				is_backup_down_ = true;
			} else {
				// Shutting down the primary server. It gets partitioned.
				ShutDown();
			}
		}
	}
}

std::thread PrimaryServerBackEnd::GetHeartBeatThread() {
	return std::thread([=] {GetHeartBeat();});

}

/*
 * Register the backup server to the member list
 */
void PrimaryServerBackEnd::SetBackupServerFrontEnd(
		BackupServerFrontEnd *backup_server_frontend) {
	backup_server_frontend_ = backup_server_frontend;
	Member t = { 2, backup_server_frontend_->get_ip(),
			backup_server_frontend_->get_port(), -1, 0 };
	memberList.push_back(t);
}

/*
 * Register the witness server to the member list
 */
void PrimaryServerBackEnd::SetWitnessServer(
		WitnessServerFrontEnd *witness_server) {
	witness_server_ = witness_server;
	Member t =
			{ 3, witness_server->get_ip(), witness_server->get_port(), -1, 0 };
	memberList.push_back(t);
}

/*
 * backup server informs the primary server it recovered and joined the group.
 */
bool PrimaryServerBackEnd::BringUpBackUp() {
	std::cout << "Backup server recover..." << std::endl;
	is_backup_down_ = false;
	return true;

}

/*
 * primary server stop running.
 */
void PrimaryServerBackEnd::ShutDown() {
	running_ = false;
}

/*
 * debug purpose function to set the primary has no response.
 */
void PrimaryServerBackEnd::SetNoResponse(bool no_response) {
	no_response_ = no_response;
}

int main() {
	//setup the group with known host_ips and port_nums.
	//server will be initialized in the thread
	PrimaryServerBackEnd primaryBackend(8072);
	WitnessServerFrontEnd witnessServer("127.0.0.1", 8071);
	BackupServerFrontEnd backupServer("127.0.0.1", 8070);
	primaryBackend.SetWitnessServer(&witnessServer);
	primaryBackend.SetBackupServerFrontEnd(&backupServer);

	//register the service
	rpc::server srv(primaryBackend.getPortNum());
	srv.bind("WriteFile",
			[&primaryBackend](const std::string &file_name,
					const std::string &file_content) {
				return primaryBackend.WriteFile(file_name, file_content);
			});

	srv.bind("ReadFile", [&primaryBackend](std::string file_name) {
		return primaryBackend.ReadFile(file_name);
	});

	srv.bind("BringUpBackUp", [&primaryBackend]() {
		return primaryBackend.BringUpBackUp();
	});
	std::cout << "Primary server is running..." << std::endl;
	srv.suppress_exceptions(true); //turn the exception to error response
	srv.run();

}

