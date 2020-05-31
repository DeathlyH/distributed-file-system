#include "primary.h"

#include <chrono>



PrimaryServerBackEnd::PrimaryServerBackEnd(int portNum) {
	this->portNum = portNum;
	this->log_top = 0;
	this->log_cp = 0;
	this->log_ap = 0;
	this->log_lb = 0;
	this->log_glb = 0;
	this->running_ = true;
	commiting_logs_thread_ = GetCommitingLogsThread();
	//backupMap = std::map<std::string, BackupServerDummy>();
	//log_map = std::map<int, LogRecord>() ;



}


//PrimaryServerBackEnd::PrimaryServerBackEnd() :
//		running_(true), commiting_logs_thread_(GetCommitingLogsThread()) {
//}

PrimaryServerBackEnd::~PrimaryServerBackEnd() {
	running_ = false;
	commiting_logs_thread_.join();
}

std::string PrimaryServerBackEnd::ReadFile(const std::string &file_name) {
//	if (promised_time_ < GetCurrentTimestamp()) {
//		promised_time_ = backup_server_frontend_->GetPromiseTime();
//	}
	std::ifstream file(file_name);
	std::string file_content;
	file >> file_content;
	file.close();
	return file_content;
}

//void PrimaryServerBackEnd::SetBackupServerFrontEnd(
//		BackupServerFrontEnd *backup_server_frontend) {
//	backup_server_frontend_ = backup_server_frontend;
//}

void PrimaryServerBackEnd::InsertRecordLog(const LogRecord &log_record) {
	log_record_list_mtx_.lock();
	log_record_list_.push_back(log_record);
	log_record_list_mtx_.unlock();
}

void PrimaryServerBackEnd::CommitLogs() {
	while (running_) {
		log_record_list_mtx_.lock();
		while (!log_record_list_.empty()) {
			const LogRecord &log = log_record_list_.front();
			std::ofstream file(log.file_name);
			file << log.file_content;
			file.close();
			log_record_list_.pop_front();
		}
		log_record_list_mtx_.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

std::thread PrimaryServerBackEnd::GetCommitingLogsThread() {
	return std::thread([=] {CommitLogs();});
}

/*
 * Let the backups check if they can do the backup.
 */
int PrimaryServerBackEnd::Prepare(LogRecord logRecord) {
	int vote = 0;

	std::cout << "Prepare for log "<<logRecord.log_id << ". \n";
	for (std::map<std::string, BackupServerDummy>::iterator it =
			backupMap.begin(); it != backupMap.end(); ++it) {

		if (it->second.CanCommit(logRecord)) {
			vote += 1;
		}
	}
	std::cout << "Get Votes "<<vote << ". \n";
	if (vote == int(backupMap.size())) {
		std::cout << "Prepare success\n";
		return true;
	}
	return false;
}

int PrimaryServerBackEnd::Commit(LogRecord logRecord) {
	std::cout << "Commit log " <<logRecord.log_id<<std::endl;
	for (std::map<std::string, BackupServerDummy>::iterator it = backupMap.begin();
			it != backupMap.end(); ++it) {
		it->second.DoCommit(logRecord);
	}
	//doCommit for the primary also.
	this->DoCommit(logRecord);
	return 0;
}

int PrimaryServerBackEnd::Abort(LogRecord logRecord) {
	std::cout << "Abort log " <<logRecord.log_id<<std::endl;
	for (std::map<std::string, BackupServerDummy>::iterator it = backupMap.begin();
			it != backupMap.end(); ++it) {
		it->second.DoAbort(logRecord);
	}
	//doAbort for the primary also
	this->DoAbort(logRecord);
	return 0;
}

bool PrimaryServerBackEnd::DoCommit(LogRecord logRecord) {
	log_mutex.lock();

	log_map[logRecord.log_id].status = 1;//COMMIT;

	log_mutex.unlock();

	return true;
}

bool PrimaryServerBackEnd::DoAbort(LogRecord logRecord) {
	log_mutex.lock();

	log_map[logRecord.log_id].status = 2;//ABORT;

	log_mutex.unlock();

	return true;
}

/*
 * When try to write a file, do the 2phase commit.
 */
bool PrimaryServerBackEnd::WriteFile(const std::string &file_name,
		const std::string &file_content) {
	std::cout << "FrontEnd calls WriteFile() " << file_name << ". \n";

	log_mutex.lock();
	log_top +=1;
	log_mutex.unlock();


	LogRecord logRecord = {.log_id = log_top,
			                      .operation_name = "WriteFile",
								  .file_name = file_name,
								  .file_content = file_content,
								  .status = HARP_NONE};
	std::cout << "Created log for WriteFile() " << file_name << ". \n";


	if (this->Prepare(logRecord)) {
		this->Commit(logRecord);
		return true;
	} else {
		this->Abort(logRecord);
		return false;
	}

	return true;
}

void PrimaryServerBackEnd::RegisterBackup(std::string backup_ip, int port_num) {
	std::cout<<"register backupserver at ip: " << backup_ip <<"port:"<<port_num<<std::endl;
	BackupServerDummy backupServer (backup_ip, port_num);

	backupMap.insert(std::pair<std::string, BackupServerDummy>(backup_ip, backupServer));
	std::cout<<"register backupserver finished " <<std::endl;
	return;
}

/*
 * The backup check the server about the decision of an action.
 *
 * @return : Abort or Commit
 */
int PrimaryServerBackEnd::Poll(LogRecord logRecord) {
	//check the decsion and return the result
	return 1; //etc. return commit for now.
}

int main() {
	// Initialize backup server.
	PrimaryServerBackEnd primaryBackend(8080);

	rpc::server srv(primaryBackend.getPortNum());
	srv.bind("WriteFile",
			[&primaryBackend](const std::string &file_name,
					const std::string &file_content) {
				return primaryBackend.WriteFile(file_name, file_content);
			});

	srv.bind("RegisterBackup", [&primaryBackend](std::string backup_ip, int port_num){
		 primaryBackend.RegisterBackup(backup_ip, port_num);
	});

	std::cout<< "Primary Server is running..." <<std::endl;
//	srv.bind("ReadFile", &PrimaryServerFrontEnd::ReadFile);
//	srv.bind("WriteFile", &PrimaryServerFrontEnd::WriteFile);
//	srv.bind("CreateFile", &PrimaryServerFrontEnd::CreateFile);
//	srv.bind("DeleteFile", &PrimaryServerFrontEnd::DeleteFile);
	srv.suppress_exceptions(true); //turn the exception to error response
	srv.run();

}
