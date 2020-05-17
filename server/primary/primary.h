#ifndef primary
#define primary

#include "../../common/common.h"
#include "../backup/backup.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>

// Definition of the back end of the primary server. When it gets some requests, it talks to the
// backup server (front end) and then comes to a decision.
class PrimaryServerBackEnd {
public:
  bool CreateFile(const std::string& file_name);
  std::string ReadFile(const std::string& file_name);
  bool WriteFile(const std::string& file_name, const std::string& file_content);
  void SetBackupServerFrontEnd(BackupServerFrontEnd* backup_server_frontend);
private:
  void InsertRecordLog(const LogRecord& log_record);
  std::list<LogRecord> log_record_list_;
  long GetPromisedTimeFromBackup();
  bool InformBackupToWriteFile(const std::string& file_name, const std::string& file_content);
  BackupServerFrontEnd* backup_server_frontend_;
  int next_available_log_id_;
  int view_number_;
  int commit_point_;
  long promised_time_;
};


// Definition of the front end of the primary server. It receives the requests from the client, and
// forwards them to the backend server. After the backend finishes processing the requests, the front
// end returns them back to the client.
// Some possible ways of communication are RPC or network.
class PrimaryServerFrontEnd {
public:
  explicit PrimaryServerFrontEnd(PrimaryServerBackEnd* primary_server_backend);
  bool Start();
  bool CreateFile(const std::string& file_name);
  std::string ReadFile(const std::string& file_name);
  bool WriteFile(const std::string& file_name, const std::string& file_content);

private:
  PrimaryServerBackEnd* primary_server_backend_;
};


#endif
