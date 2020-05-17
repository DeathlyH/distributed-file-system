#ifndef common
#define common


#include <string>

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
  PayLoad(const LogRecord& log_record, int commit_point);
  LogRecord log_record;
  int commit_point;
};

#endif
