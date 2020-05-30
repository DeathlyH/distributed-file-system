#include "witness.h"

bool WitnessServer::RequestViewChange(bool is_primary_server) {
  if (is_primary_server && !backup_requested_view_change_) {
    primary_requested_view_change_ = true;
    return true;
  }
  if (!is_primary_server && !primary_requested_view_change_) {
    backup_requested_view_change_ = true;
    return true;
  }
  return false;
}

bool WitnessServer::RecordLogRecords(const PayLoad& payload) {
  log_record_list_mtx_.lock();
  for (const auto& log : payload.log_record_vector) {
    log_record_list_.push_back(log);
  }
  log_record_list_mtx_.unlock();
  return true;
}

PayLoad WitnessServer::GetLogRecords() {
  PayLoad payload;
  log_record_list_mtx_.lock();
  for (const auto& log : log_record_list_) {
    payload.log_record_vector.push_back(log);
  }
  log_record_list_mtx_.unlock();
  return payload;
}
