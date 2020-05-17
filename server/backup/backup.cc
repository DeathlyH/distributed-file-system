#include "backup.h"
/***************
  Back End.
****************/
long BackupServerBackEnd::GetPromiseTime() {
  promised_time_ = 0;
  std::cout << "return promised_time " << promised_time_ << ". \n";
  return promised_time_;
}

bool BackupServerBackEnd::RequestCommit(const PayLoad& payload) {
  InsertRecordLog(payload.log_record);
  std::cout << "file name is " << payload.log_record.file_name << ". \n";
  return true;
}

void BackupServerBackEnd::InsertRecordLog(const LogRecord& log_record) {
  // Get mutex.
  log_record_list_.push_back(log_record);
}

/***************
  Front End.
 ****************/

BackupServerFrontEnd::BackupServerFrontEnd(BackupServerBackEnd* backup_server_backend): backup_server_backend_(backup_server_backend) {}

bool BackupServerFrontEnd::Start() {
  std::cout << "BackupServerFrontEnd started. \n";
  return true;
}

long BackupServerFrontEnd::GetPromiseTime() {
  std::cout << "primary server calls GetPromiseTime(). \n";
  return backup_server_backend_->GetPromiseTime();
}

bool BackupServerFrontEnd::RequestCommit(const PayLoad& payload) {
  return backup_server_backend_->RequestCommit(payload);
}
