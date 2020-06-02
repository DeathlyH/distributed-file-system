#include "backup.h"
#include <fstream>
#include "rpc/server.h"
/***************
 Back End.
 ****************/

BackupServerBackEnd::BackupServerBackEnd(int port_num) :
port_num(port_num) ,running_(true), commiting_logs_thread_(GetCommitingLogsThread()), check_view_change_thread_(
				GetViewChangeThread()){
}

BackupServerBackEnd::~BackupServerBackEnd() {
	running_ = false;
	commiting_logs_thread_.join();
	check_view_change_thread_.join();
}

void BackupServerBackEnd::Start() {
	if (!witness_server_) {
		std::cout << "Should call SetWitnessServer() first. \n";
	}
	PayLoad payload = witness_server_->GetLogRecords();
	for (const auto &log : payload.log_record_vector) {
		log_record_list_.push_back(log);
		next_available_log_id_ = log.log_id + 1;
	}
	std::cout << "Backup: next_available_log_id is " << next_available_log_id_
			<< ". \n";
}

long BackupServerBackEnd::GetPromiseTime() {
	promised_time_ = GetCurrentTimestamp();
	std::cout << "backup returns promised_time " << promised_time_ << ". \n";
	return promised_time_;
}

bool BackupServerBackEnd::RequestCommit(const PayLoad &payload) {
	last_request_time_ = GetCurrentTimestamp();
	if (payload.log_record_vector.empty()) {
		std::cout << "Receive heartbeat from primary. \n";
		return true;
	}
	for (const LogRecord &log : payload.log_record_vector) {
		InsertRecordLog(log);
		std::cout << "file name is " << log.file_name << ". \n";
		std::cout << "file content is " << log.operation_content << ". \n";
		next_available_log_id_ = log.log_id + 1;
	}
	return true;
}

void BackupServerBackEnd::Commit(const PayLoad &payload) {
	commit_point_mtx_.lock();
	commit_point_ = payload.commit_point;
	commit_point_mtx_.unlock();
}

void BackupServerBackEnd::InsertRecordLog(const LogRecord &log_record) {
	log_record_list_mtx_.lock();
	log_record_list_.push_back(log_record);
	log_record_list_mtx_.unlock();
}

void BackupServerBackEnd::CommitLogs() {
	while (running_) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		log_record_list_mtx_.lock();
		while (!log_record_list_.empty()) {
			std::cout << "backup trying to CommitLogs \n";
			const LogRecord &log = log_record_list_.front();
			commit_point_mtx_.lock();
			if (commit_point_ < log.log_id) {
				commit_point_mtx_.unlock();
				break;
			}
			commit_point_mtx_.unlock();
			std::ofstream file(log.file_name);
			file << log.operation_content;
			file.close();
			log_record_list_.pop_front();
		}
		log_record_list_mtx_.unlock();
	}
}

std::thread BackupServerBackEnd::GetViewChangeThread() {
	return std::thread([=] {
		while (running_) {
			std::this_thread::sleep_for (std::chrono::seconds(1));
			if (!is_primary_ && GetCurrentTimestamp() - last_request_time_ > 10) {
				PayLoad payload;
				payload.is_primary_server = false;
				view_number_++;
				payload.view_number = view_number_;
				if (witness_server_->RequestViewChange(payload)) {
					std::cout << "backup becomes primary. \n";
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

void BackupServerBackEnd::ShutDown() {
	running_ = false;
}

std::thread BackupServerBackEnd::GetCommitingLogsThread() {
	return std::thread([=] {CommitLogs();});
}

void BackupServerBackEnd::SetWitnessServer(
		WitnessServerFrontEnd *witness_server) {
	witness_server_ = witness_server;
}

void BackupServerBackEnd::Demote() {
	is_primary_ = false;
}

std::string BackupServerBackEnd::ReadFile(const std::string &file_name) {
	if (!is_primary_) {
		std::cout << "Error. should call primary server. \n";
		return "";
	}
	std::ifstream file(file_name);
	std::string file_content, temp;
	while (file >> temp) {
		file_content += temp;
	}
	file.close();
	return file_content;
}

bool BackupServerBackEnd::WriteFile(const std::string &file_name,
		const std::string &file_content) {
	if (!is_primary_) {
		std::cout << "Error. should call primary server. \n";
		return false;
	}
	LogRecord log(GetCurrentTimestamp(), next_available_log_id_++, "WriteFile",
			file_name, file_content, "backup");
	InsertRecordLog(log);
	commit_point_ = next_available_log_id_;
	next_available_log_id_++;
	return true;
}

int main() {

	BackupServerBackEnd backend(8070);
	WitnessServerFrontEnd witnessFrontEnd("127.0.0.1", 8071);
	backend.SetWitnessServer(&witnessFrontEnd);



	rpc::server srv(backend.getPortNum());

    srv.bind("RequestCommit", [&backend](PayLoad payload){
    	return backend.RequestCommit(payload);
    });

    srv.bind("GetPromiseTime", [&backend](){
    	return backend.GetPromiseTime();
    });

    srv.bind("Commit", [&backend](PayLoad payload){
    	return backend.Commit(payload);
    });



    srv.bind("Demote", [&backend](PayLoad payload){
    	return backend.Demote();
    });


    srv.bind("WriteFile", [&backend](std::string file_name, std::string file_content){
    	backend.WriteFile(file_name, file_content);
    });

    srv.bind("ReadFile", [&backend](std::string file_name){
    	backend.ReadFile(file_name);
    });

	srv.suppress_exceptions(true); //turn the exception to error response
	std::cout << "BackupServer started. \n";
	srv.run();

	return 0;
}
