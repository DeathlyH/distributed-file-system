#ifndef backup
#define backup
#include <iostream>
#include <list>
#include "../../common/common.h"

// Definition of the back end of the backup server. It processes the message from the primary server.
class BackupServerBackEnd {
public:
  BackupServerBackEnd() = default;
  long GetPromiseTime();
  bool RequestCommit(const PayLoad& payload);
private:
  void InsertRecordLog(const LogRecord& log_record);
  long promised_time_;
  std::list<LogRecord> log_record_list_;
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
private:
  BackupServerBackEnd* backup_server_backend_;
};

#endif

