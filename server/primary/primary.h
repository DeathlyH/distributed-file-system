#ifndef primary
#define primary

#include "../../common/common.h"
#include "../backup/backup.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <mutex>
#include <thread>
#include "rpc/server.h"
//#include "rpc/client.h"
#include "rpc/rpc_error.h"

#include "backupServerDummy.h"

//class BackupServerStub {
//public:
//	BackupServerStub(std::string backup_ip, int port_num) {
//
//		this->backup_ip = backup_ip;
//		this->port_num = port_num;
//	}
//
//	bool CanCommit(LogRecord logRecord) {
//		rpc::client c(backup_ip, port_num);
//		return c.call("CanCommit", logRecord);
//	}
//
//	bool DoCommit(LogRecord logRecord) {
//		rpc::client c(backup_ip, port_num);
//		return c.call("DoCommit", logRecord);
//	}
//
//	bool DoAbort(LogRecord logRecord) {
//		rpc::client c(backup_ip, port_num);
//		return c.call("DoAbort", logRecord);
//	}
//
//private:
//	std::string backup_ip;
//	int port_num;
//
//
//};

// Definition of the back end of the primary server. When it gets some requests, it talks to the
// backup server (front end) and then comes to a decision.
class PrimaryServerBackEnd {
public:
	PrimaryServerBackEnd(int portNum) ;
	~PrimaryServerBackEnd();
	int getPortNum() {
		return portNum;
	}
	std::string ReadFile(const std::string &file_name);
	bool WriteFile(const std::string &file_name,
			const std::string &file_content);
	//void SetBackupServerFrontEnd(BackupServerFrontEnd* backup_server_frontend);
	int Prepare(LogRecord);
	int Commit(LogRecord logRecord);
	int Abort(LogRecord logRecord);
	bool DoCommit(LogRecord logRecord);
	bool DoAbort(LogRecord logRecord);

	void RegisterBackup(std::string backup_ip, int port_num);
	void RegisterWitness(std::string witness_ip, int port_num);
	int Poll(LogRecord logRecord);

private:
	int portNum;
	int log_top;
	int log_cp;
	int log_ap;
	int log_lb;
	int log_glb;

	std::mutex log_mutex;
	std::map<int, LogRecord> log_map;

	void InsertRecordLog(const LogRecord &log_record);
	long GetPromisedTimeFromBackup();
	bool InformBackupToWriteFile(const std::string &file_name,
			const std::string &file_content);
	// Commits the logs to the file system.
	void CommitLogs();
	// Must be called.
	std::thread GetCommitingLogsThread();

	// A mutex to protect log_record_list_.
	std::mutex log_record_list_mtx_;
	std::list<LogRecord> log_record_list_;

//	BackupServerFrontEnd *backup_server_frontend_;
	// True if the primary server is running.
	bool running_;
	std::thread commiting_logs_thread_;

	int view_number_ = -1;
	int commit_point_ = -1;
	long promised_time_;

	std::list<BackupServerDummy> backupList; //In the coordinator's eye witness and backup are the same
	std::map<std::string, BackupServerDummy> backupMap;
};

#endif
// Definition of the front end of the primary server. It receives the requests from the client, and
// forwards them to the backend server. After the backend finishes processing the requests, the front
// end returns them back to the client.
// Some possible ways of communication are RPC or network.
//class PrimaryServerFrontEnd {
//public:
//  explicit PrimaryServerFrontEnd(PrimaryServerBackEnd* primary_server_backend);
//  bool Start();
//  std::string ReadFile(const std::string& file_name);
//  bool WriteFile(const std::string &file_name, const std::string &file_content);
//  int CreateFile(const std::string & file_name);
//  int DeleteFile(const std::string & file_name);
//
//private:
//  PrimaryServerBackEnd* primary_server_backend_;
//};


