#include "backup.h"
#include <fstream>

/***************
  Back End.
****************/

BackupServerBackEnd::BackupServerBackEnd():running_(true),
                                           commiting_logs_thread_(GetCommitingLogsThread()) {}

BackupServerBackEnd::~BackupServerBackEnd() {
  running_ = false;
  commiting_logs_thread_.join();
}

long BackupServerBackEnd::GetPromiseTime() {
  promised_time_ = GetCurrentTimestamp();
  std::cout << "return promised_time " << promised_time_ << ". \n";
  return promised_time_;
}

bool BackupServerBackEnd::RequestCommit(const PayLoad& payload) {
  if (payload.log_record_vector.empty()) {
    std::cout << "Receive heartbeat from primary. \n";
    return true;
  }
  for (const LogRecord& log : payload.log_record_vector) {
    InsertRecordLog(log);
    std::cout << "file name is " << log.file_name << ". \n";
    std::cout << "file content is " << log.operation_content << ". \n";
  }
  return true;
}

void BackupServerBackEnd::Commit(const PayLoad& payload) {
  commit_point_mtx_.lock();
  commit_point_ = payload.commit_point;
  commit_point_mtx_.unlock();
}

void BackupServerBackEnd::InsertRecordLog(const LogRecord& log_record) {
  log_record_list_mtx_.lock();
  log_record_list_.push_back(log_record);
  log_record_list_mtx_.unlock();
}

void BackupServerBackEnd::CommitLogs() {
  while (running_) {
    log_record_list_mtx_.lock();
    while (!log_record_list_.empty()) {
      std::cout << "backup trying to CommitLogs \n";
      const LogRecord& log = log_record_list_.front();
      commit_point_mtx_.lock();
      if (commit_point_ < log.log_id) {
        commit_point_mtx_.unlock();
        break;
      }
      commit_point_mtx_.unlock();
      std::ofstream file("/Users/Jiaming/Desktop/test/file2.txt");
      file << log.operation_content;
      file.close();
      log_record_list_.pop_front();
    }
    log_record_list_mtx_.unlock();
    std::this_thread::sleep_for (std::chrono::seconds(1));
  }
}

std::thread BackupServerBackEnd::GetCommitingLogsThread() {
  return std::thread( [=] { CommitLogs(); } );
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
  std::cout << "primary server calls RequestCommit(). \n";
  return backup_server_backend_->RequestCommit(payload);
}

void BackupServerFrontEnd::Commit(const PayLoad& payload) {
  std::cout << "primary server calls Commit(). \n";
  if (!backup_server_backend_) {
    std::cout << "primary server calls Commit() NULLLLLL. \n";
  }
  backup_server_backend_->Commit(payload);
}
