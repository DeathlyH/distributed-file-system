#include "primary.h"
/***************
  Back End.
****************/
bool PrimaryServerBackEnd::CreateFile(const std::string& file_name) {
  std::ofstream file(file_name);
  file.close();
  return true;
}

bool PrimaryServerBackEnd::WriteFile(const std::string& file_name, const std::string& file_content) {
  std::cout << "FrontEnd calls WriteFile() " << file_name << ". \n";
  LogRecord log_record(0, next_available_log_id_++, "WriteFile", file_name, file_content, "primary");
  PayLoad payload(log_record, 0);
  if (backup_server_frontend_->RequestCommit(payload) == false) {
    return false;
  }
  InsertRecordLog(log_record);
  std::ofstream file(file_name);
  file << file_content;
  file.close();
  return true;
}

std::string PrimaryServerBackEnd::ReadFile(const std::string& file_name) {
  promised_time_ = backup_server_frontend_->GetPromiseTime();
  std::ifstream file(file_name);
  std::string file_content;
  file >> file_content;
  file.close();
  return file_content;
}

void PrimaryServerBackEnd::SetBackupServerFrontEnd(BackupServerFrontEnd* backup_server_frontend) {
  backup_server_frontend_ = backup_server_frontend;
}

void PrimaryServerBackEnd::InsertRecordLog(const LogRecord& log_record) {
  // Get mutex.
  log_record_list_.push_back(log_record);
}



/***************
 Front End.
****************/

PrimaryServerFrontEnd::PrimaryServerFrontEnd(PrimaryServerBackEnd* primary_server_backend):primary_server_backend_(primary_server_backend) {}

bool PrimaryServerFrontEnd::Start() {
  // Set up rpc or network connections.
  std::cout << "PrimaryServerFrontEnd Started.\n";
  return true;
}

bool PrimaryServerFrontEnd::CreateFile(const std::string& file_name) {
  return primary_server_backend_->CreateFile(file_name);
}

bool PrimaryServerFrontEnd::WriteFile(const std::string& file_name, const std::string& file_content) {
  std::cout << "Client calls WriteFile() " <<file_name << ". \n";
  return primary_server_backend_->WriteFile(file_name, file_content);
}

std::string PrimaryServerFrontEnd::ReadFile(const std::string& file_name) {

  return primary_server_backend_->ReadFile(file_name);
}
