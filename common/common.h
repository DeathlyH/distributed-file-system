#ifndef common
#define common

#include <ctime>
#include <string>
#include <vector>
#include <rpc/msgpack.hpp>

long GetCurrentTimestamp();

struct LogRecord {
	LogRecord() = default;
	LogRecord(long timestamp, int log_id, const std::string &operation_name,
			const std::string &file_name, const std::string &operation_content,
			const std::string &log_owner);
	long timestamp;
	int log_id;
	std::string operation_name;
	std::string file_name;
	std::string operation_content;
	std::string log_owner;
			MSGPACK_DEFINE(log_id, operation_name, file_name, operation_content, log_owner)
	;
};

struct PayLoad {
	PayLoad() = default;
	PayLoad(const std::vector<LogRecord> &log_record_vector, int commit_point, int log_ap);
	std::vector<LogRecord> log_record_vector;
	int commit_point = 0;
	int log_ap = 0;

	// Used during a view change.
	bool is_primary_server;
	int view_number;
			MSGPACK_DEFINE(log_record_vector, commit_point, log_ap, is_primary_server, view_number)
	;

};

#endif
