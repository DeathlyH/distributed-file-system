#ifndef primary
#define primary

#include "../../common/common.h"
#include "../backup/backup.h"
#include "../witness/witness.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <mutex>
#include <thread>

// Definition of the back end of the primary server. When it gets some requests, it talks to the
// backup server (front end) and then comes to a decision.
class PrimaryServerBackEnd {
public:
  PrimaryServerBackEnd();
  ~PrimaryServerBackEnd();
  // talk to witness and get missing logs, if there is any.
  void Start();
  std::string ReadFile(const std::string& file_name);
  bool WriteFile(const std::string& file_name, const std::string& file_content);
  void SetBackupServerFrontEnd(BackupServerFrontEnd* backup_server_frontend);
  void SetWitnessServer(WitnessServer* witness_server);
  void BringUpBackUp();
  // Only used in testing.
  void SetNoResponse(bool no_response);
private:
  void InsertRecordLog(const LogRecord& log_record);
  long GetPromisedTimeFromBackup();
  bool InformBackupToWriteFile(const std::string& file_name, const std::string& file_content);
  // Commits the logs to the file system.
  void CommitLogs();
  void ShutDown();
  // Must be called.
  std::thread GetCommitingLogsThread();
  std::thread GetHeartBeatThread();

  // A mutex to protect log_record_list_ and last_request_time_.
  std::mutex log_record_list_mtx_;
  std::list<LogRecord> log_record_list_;
  long last_request_time_ = 0;

  BackupServerFrontEnd* backup_server_frontend_;
  // True if the primary server is running.
  bool running_;
  std::thread commiting_logs_thread_;
  std::thread heart_beat_thread_;
  int next_available_log_id_ = 0;
  int view_number_ = 0;
  int commit_point_ = -1;
  long promised_time_ = 0;
  bool is_backup_down_ = false;
  bool no_response_ = false;
  WitnessServer* witness_server_;
};


// Definition of the front end of the primary server. It receives the requests from the client, and
// forwards them to the backend server. After the backend finishes processing the requests, the front
// end returns them back to the client.
// Some possible ways of communication are RPC or network.
class PrimaryServerFrontEnd {
public:
  explicit PrimaryServerFrontEnd(PrimaryServerBackEnd* primary_server_backend);
  bool Start();
  std::string ReadFile(const std::string& file_name);
  bool WriteFile(const std::string& file_name, const std::string& file_content);
  void BringUpBackUp();

private:
  PrimaryServerBackEnd* primary_server_backend_;
};


#endif
