#ifndef backup
#define backup
#include <iostream>
#include <list>
#include <mutex>
#include <thread>
#include "../../common/common.h"
#include "../witness/witness.h"

// Definition of the back end of the backup server. It processes the message from the primary server.
class BackupServerBackEnd {
public:
  BackupServerBackEnd();
  ~BackupServerBackEnd();
  long GetPromiseTime();
  bool RequestCommit(const PayLoad& payload);
  void Commit(const PayLoad& payload);
  void SetWitnessServer(WitnessServer* witness_server);
  
  // Commits the logs to the file system.
  void CommitLogs();
  // Must be called.
  std::thread GetCommitingLogsThread();

private:
  // True if the second server is running.
  bool running_;
  
  std::thread commiting_logs_thread_;
  void InsertRecordLog(const LogRecord& log_record);
  int view_number_ = -1;
  int commit_point_ = -1;
  long promised_time_;
  // A mutex to protect log_record_list_.
  std::mutex log_record_list_mtx_;
  std::list<LogRecord> log_record_list_;
  std::mutex commit_point_mtx_;
  WitnessServer* witness_server_;
};

// Definition of the front end of the backup server. It talks to the back end of the primary server.
// Uses RPC or network.
class BackupServerFrontEnd {
public:
  explicit BackupServerFrontEnd(BackupServerBackEnd* backup_server_backend);
  bool Start();
  // If true, the backup server agrees to commit this log.
  bool RequestCommit(const PayLoad& payload);
  long GetPromiseTime();
  void Commit(const PayLoad& payload);
  void SetNoResponse(bool no_response);
private:
  BackupServerBackEnd* backup_server_backend_;
  bool no_response_ = false;
};

#endif

