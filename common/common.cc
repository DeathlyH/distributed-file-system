#include "common.h"

long GetCurrentTimestamp() {
  time_t t = std::time(0);
  return static_cast<long>(t);
}

//LogRecord::LogRecord(long timestamp, int log_id, const std::string& operation_name,
//                     const std::string& file_name, const std::string& operation_content,
//                     const std::string& log_owner) {
//  this->timestamp = timestamp;
//  this->log_id = log_id;
//  this->operation_name = operation_name;
//  this->file_name = file_name;
//  this->operation_content = operation_content;
//  this->log_owner = log_owner;
//}

//PayLoad::PayLoad(const std::vector<LogRecord>& log_record_vector, int commit_point) :
//                                                                log_record_vector(log_record_vector),
//                                                                commit_point(commit_point) {}
