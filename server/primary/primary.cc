#include "primary.h"
#include <chrono>

/***************
  Back End.
****************/

PrimaryServerBackEnd::PrimaryServerBackEnd(): running_(true),
                                              commiting_logs_thread_(GetCommitingLogsThread()),
                                              heart_beat_thread_(GetHeartBeatThread()) {}

PrimaryServerBackEnd::~PrimaryServerBackEnd() {
  running_ = false;
  commiting_logs_thread_.join();
  heart_beat_thread_.join();
}


bool PrimaryServerBackEnd::WriteFile(const std::string& file_name, const std::string& file_content) {
  std::cout << "FrontEnd calls WriteFile() " << file_name << ". \n";
  LogRecord log_record(0, next_available_log_id_++, "WriteFile", file_name, file_content, "primary");
  PayLoad payload({log_record}, 0);
  if (!is_backup_down_) {
    if (backup_server_frontend_->RequestCommit(payload) == false) {
      return false;
    }
  } else {
    if (witness_server_->RecordLogRecords(payload) == false)
      return false;
  }

  InsertRecordLog(log_record);
  return true;
}

std::string PrimaryServerBackEnd::ReadFile(const std::string& file_name) {
  if (promised_time_ < GetCurrentTimestamp()) {
    if (!is_backup_down_) {
      promised_time_ = backup_server_frontend_->GetPromiseTime();
    }
  }
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
  log_record_list_mtx_.lock();
  log_record_list_.push_back(log_record);
  last_request_time_ = GetCurrentTimestamp();
  log_record_list_mtx_.unlock();
}

void PrimaryServerBackEnd::CommitLogs() {
  while (running_) {
    std::this_thread::sleep_for (std::chrono::seconds(1));
    log_record_list_mtx_.lock();
    while (!log_record_list_.empty()) {
      const LogRecord& log = log_record_list_.front();
      std::ofstream file(log.file_name);
      file << log.operation_content;
      file.close();
      commit_point_ = log.log_id;
      log_record_list_.pop_front();
    }
    log_record_list_mtx_.unlock();
    if (!is_backup_down_) {
      backup_server_frontend_->Commit({{}, commit_point_});
    }
  }
}

std::thread PrimaryServerBackEnd::GetCommitingLogsThread() {
  return std::thread( [=] { CommitLogs(); } );
}

std::thread PrimaryServerBackEnd::GetHeartBeatThread() {
  return std::thread( [=] {
    while (running_) {
      // Emits heartbeat at least every 2 seconds.
      if (GetCurrentTimestamp() - last_request_time_ > 2) {
        PayLoad payload;
        if (is_backup_down_) {
          if (witness_server_->RecordLogRecords(payload)) {
            log_record_list_mtx_.lock();
            last_request_time_ = GetCurrentTimestamp();
            log_record_list_mtx_.unlock();
          }
        }
        else {
          if (backup_server_frontend_->RequestCommit(payload) == true) {
            log_record_list_mtx_.lock();
            last_request_time_ = GetCurrentTimestamp();
            log_record_list_mtx_.unlock();
          }
        }
      }
      std::this_thread::sleep_for (std::chrono::seconds(1));
      
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
          // Shutting down the primary server.
          ShutDown();
        }
      }
    }
  } );
}

void PrimaryServerBackEnd::SetWitnessServer(WitnessServer* witness_server) {
  witness_server_ = witness_server;
}

void PrimaryServerBackEnd::BringUpBackUp() {
  is_backup_down_ = false;
}

void PrimaryServerBackEnd::ShutDown() {
  running_ = false;
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


bool PrimaryServerFrontEnd::WriteFile(const std::string& file_name, const std::string& file_content) {
  std::cout << "Client calls WriteFile() " <<file_name << ". \n";
  return primary_server_backend_->WriteFile(file_name, file_content);
}

std::string PrimaryServerFrontEnd::ReadFile(const std::string& file_name) {

  return primary_server_backend_->ReadFile(file_name);
}

void PrimaryServerFrontEnd::BringUpBackUp() {
  primary_server_backend_->BringUpBackUp();
}
