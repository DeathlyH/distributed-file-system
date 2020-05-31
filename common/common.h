#ifndef __COMMON__
#define __COMMON__

#include <ctime>
#include <string>
#include <vector>
#include "msgpack.hpp"


#define HARP_COMMIT (1)
#define HARP_ABORT (2)
#define HARP_NONE (0)

long GetCurrentTimestamp();

enum EventStatus{
	NONE,
	COMMIT,
	ABORT
};

typedef struct  {
//  LogRecord(long timestamp, int log_id, const std::string& operation_name,
//            const std::string& file_name, const std::string& operation_content,
//            const std::string& log_owner);
//  long timestamp;
  int log_id;
  std::string operation_name;
  std::string file_name;
  std::string file_content;
//  std::string log_owner;
  int status;
  MSGPACK_DEFINE(log_id,operation_name, file_name, file_content, status);
} LogRecord;


struct PayLoad {
//  PayLoad(const std::vector<LogRecord>& log_record_vector, int commit_point);
  std::vector<LogRecord> log_record_vector;
  int commit_point;
};

#endif
