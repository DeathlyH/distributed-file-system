#include "backup.h"
/***************
 Back End.
 ****************/
//long BackupServerBackEnd::GetPromiseTime() {
//	promised_time_ = GetCurrentTimestamp();
//	std::cout << "return promised_time " << promised_time_ << ". \n";
//	return promised_time_;
//}
//bool BackupServerBackEnd::RequestCommit(const PayLoad &payload) {
//	for (const LogRecord &log : payload.log_record_vector) {
//		InsertRecordLog(log);
//		std::cout << "file name is " << log.file_name << ". \n";
//	}
//	return true;
//}
//
//void BackupServerBackEnd::InsertRecordLog(const LogRecord &log_record) {
//	// Get mutex.
//	log_record_list_.push_back(log_record);
//}

/***************
 Front End.
 ****************/

//BackupServerFrontEnd::BackupServerFrontEnd(
//		BackupServerBackEnd *backup_server_backend) :
//		backup_server_backend_(backup_server_backend) {
//}
//
//bool BackupServerFrontEnd::Start() {
//	std::cout << "BackupServerFrontEnd started. \n";
//	return true;
//}
//
//long BackupServerFrontEnd::GetPromiseTime() {
//	std::cout << "primary server calls GetPromiseTime(). \n";
//	return backup_server_backend_->GetPromiseTime();
//}
//
//bool BackupServerFrontEnd::RequestCommit(const PayLoad &payload) {
//	return backup_server_backend_->RequestCommit(payload);
//}
///*
// * backup should return if it awares the request 2 phase commit, phase 1
// */
//int BackupServerFrontEnd::CanCommit(){
//	//I can only commit if you're the real leader.
//
//	return 1; //assume always true at first
//
//}
//
///*
// *
// */
//int BackupServerFrontEnd::DoCommit(){
//	return 1; //always successful
//}
//
//
///*
// * give up
// */
//int BackupServerFrontEnd::DoAbort(){
//	return 1;
//}
BackupServerBackEnd::BackupServerBackEnd(int portNum) {
	this->portNum = portNum;
	this->log_top = 0;
	this->log_cp = 0;
	this->log_ap = 0;
	this->log_lb = 0;
	this->log_glb = 0;
	//temporary set all the member to default value;

}

int BackupServerBackEnd::CanCommit(LogRecord logRecord) {
	//write the record to the log;

	std::cout<<"CanCommit called for log "<<logRecord.log_id<<std::endl;
	log_mutex.lock();

	log_map.insert(std::pair<int, LogRecord>(logRecord.log_id, logRecord));

	log_mutex.unlock();

	std::cout<<"CanCommit finished for log "<<logRecord.log_id<<std::endl;

	return 1;
}

int BackupServerBackEnd::DoCommit(LogRecord logRecord) {

	std::cout<<"DoCommit called for log "<<logRecord.log_id<<std::endl;
	log_mutex.lock();

	log_map[logRecord.log_id].status = 1;//COMMIT;

	log_mutex.unlock();

	return 1;
}

int BackupServerBackEnd::DoAbort(LogRecord logRecord) {
	std::cout<<"DoAbort called for log "<<logRecord.log_id<<std::endl;
	log_mutex.lock();

	log_map[logRecord.log_id].status = 2;//ABORT;

	log_mutex.unlock();

	return 1;
}

/*
 * expose the 2 phase commit interface to the primary server.
 */
void BackupServerBackEnd::StartService() {
//	rpc::server srv(this->portNum);
//	srv.bind("DoCommit", [this](LogRecord logRecord) {
//		this->DoCommit(logRecord);
//	});
//	srv.bind("DoAbort", [this](LogRecord logRecord) {
//		this->DoAbort(logRecord);
//	});
//
//	srv.bind("CanCommit", [this](LogRecord logRecord) {
//		this->CanCommit(logRecord);
//	});
//
//	srv.suppress_exceptions(true); //turn the exception to error response
//	std::cout << "BackupServer started. \n";
//	srv.run();
}

///*
// * set the primary server
// */
//void BackupServerBackEnd::SetPrimary(std::string primary_ip, int port_num){
//	primary_server_ip = primary_ip;
//	primary_port_num = port_num;
//}

void BackupServerBackEnd::AppendTentative(LogRecord logRecord) {
//	std::ofstream myfile;
//	myfile.open(logFilename.c_str());
//	myfile << logRecord;
//	myfile.close();
	return;
}

int main() {

	BackupServerBackEnd backend(8070);
	rpc::server srv(backend.getPortNum());
	srv.bind("DoCommit", [&backend](LogRecord logRecord) {
		return backend.DoCommit(logRecord);
	});
	srv.bind("DoAbort", [&backend](LogRecord logRecord) {
		return backend.DoAbort(logRecord);
	});

	srv.bind("CanCommit", [&backend](LogRecord logRecord) {
		return backend.CanCommit(logRecord);
	});

	srv.suppress_exceptions(true); //turn the exception to error response
	std::cout << "BackupServer started. \n";
	srv.run();

	return 0;
}
