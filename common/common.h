#ifndef common
#define common

#include <ctime>
#include <string>
#include <vector>

long GetCurrentTimestamp();

struct LogRecord {
  LogRecord(long timestamp, int log_id, const std::string& operation_name,
            const std::string& file_name, const std::string& operation_content,
            const std::string& log_owner);
  long timestamp;
  int log_id;
  std::string operation_name;
  std::string file_name;
  std::string operation_content;
  std::string log_owner;
};


struct PayLoad {
  PayLoad() = default;
  PayLoad(const std::vector<LogRecord>& log_record_vector, int commit_point);
  std::vector<LogRecord> log_record_vector;
  int commit_point = 0;
  
  // Used during a view change.
  bool is_primary_server;
  int view_number;
};

#endif
