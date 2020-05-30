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
  // talk to witness and get missing logs, if there is any.
  void Start();
  long GetPromiseTime();
  bool RequestCommit(const PayLoad& payload);
  void Commit(const PayLoad& payload);
  void SetWitnessServer(WitnessServer* witness_server);
  
  // Commits the logs to the file system.
  void CommitLogs();
  // Must be called.
  std::thread GetCommitingLogsThread();
  std::thread GetViewChangeThread();
  
  // If called, the backup server which previously works as primary, will be demoted to backup
  // server.
  void Demote();
  
  // Will be used when working as primary.
  std::string ReadFile(const std::string& file_name);
  bool WriteFile(const std::string& file_name, const std::string& file_content);
private:
  void ShutDown();
  // True if the second server is running.
  bool running_;
  
  std::thread commiting_logs_thread_;
  std::thread check_view_change_thread_;
  void InsertRecordLog(const LogRecord& log_record);
  int view_number_ = 0;
  int commit_point_ = -1;
  long promised_time_ = 0;
  long last_request_time_ = GetCurrentTimestamp();
  // A mutex to protect log_record_list_.
  std::mutex log_record_list_mtx_;
  std::list<LogRecord> log_record_list_;
  std::mutex commit_point_mtx_;
  WitnessServer* witness_server_;
  bool is_primary_ = false;
  int next_available_log_id_ = 0;
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
  // Only used in testing.
  void SetNoResponse(bool no_response);
  
  void Demote();
  
  // Will be used when working as primary.
  std::string ReadFile(const std::string& file_name);
  bool WriteFile(const std::string& file_name, const std::string& file_content);
private:
  BackupServerBackEnd* backup_server_backend_;
  bool no_response_ = false;
};

#endif

