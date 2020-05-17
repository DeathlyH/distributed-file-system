#include "common.h"

LogRecord::LogRecord(long timestamp, int log_id, const std::string& operation_name,
                     const std::string& file_name, const std::string& operation_content,
                     const std::string& log_owner) {
  this->timestamp = timestamp;
  this->log_id = log_id;
  this->operation_name = operation_name;
  this->file_name = file_name;
  this->operation_content = operation_content;
  this->log_owner = log_owner;
}

PayLoad::PayLoad(const LogRecord& log_record, int commit_point) : log_record(log_record),
                                                                  commit_point(commit_point) {}
