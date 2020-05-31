#ifndef backup
#define backup
#include <iostream>
#include <list>
#include <fstream>
#include <thread>
#include <mutex>
#include <map>
#include <rpc/server.h>
#include "../../common/common.h"




// Definition of the back end of the backup server. It processes the message from the primary server.
class BackupServerBackEnd {
public:
	BackupServerBackEnd(int portNum);
	//long GetPromiseTime();
//	bool RequestCommit(const PayLoad &payload);
	int CanCommit(LogRecord logRecord);
	int DoCommit(LogRecord logRecord);
	int DoAbort(LogRecord logRecord);
	void StartService();
	int getPortNum(){
		return portNum;
	}
private:
//	void InsertRecordLog(const LogRecord &log_record);
	//long promised_time_;
	std::list<LogRecord> log_record_list_;
	std::map<int, LogRecord> log_map;
	std::mutex log_mutex;

	int log_top;
	int log_cp;
	int log_ap;
	int log_lb;
	int log_glb;

	std::string logFilename = "abcd.txt";
//	std::string primary_server_ip;
//	int primary_port_num;

	int portNum;

	void AppendTentative(LogRecord logRecord);


//	void SetPrimary(std::string primary_ip, int port_num);
};

#endif
//// Definition of the front end of the backup server. It talks to the back end of the primary server.
//// Uses RPC or network.
//class BackupServerFrontEnd {
//public:
//	explicit BackupServerFrontEnd(BackupServerBackEnd *backup_server_backend);
//	bool Start();
//	// If true, the backup server agrees to commit this log.
//	bool RequestCommit(const PayLoad &payload);
//	long GetPromiseTime();
//
//	int CanCommit();
//	int DoCommit();
//	int DoAbort();
//
//
//private:
//	BackupServerBackEnd *backup_server_backend_;
//};



