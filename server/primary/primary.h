#ifndef primary
#define primary

#include "../../common/common.h"
#include "../backup/backupFrontEnd.h"
#include "../witness/witnessFrontEnd.h"
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
	PrimaryServerBackEnd(int port_num);
	~PrimaryServerBackEnd();
	int getPortNum() {
		return port_num;
	}
	// talk to witness and get missing logs, if there is any.
	void Start();
	std::string ReadFile(const std::string &file_name);
	bool WriteFile(const std::string &file_name,
			const std::string &file_content);
	void SetBackupServerFrontEnd(BackupServerFrontEnd *backup_server_frontend);
	void SetWitnessServer(WitnessServerFrontEnd *witness_server);
	bool BringUpBackUp();

	//void PrimaryServerBackEnd::RegisterBackup(std::string backup_ip, int port_num);
	// Only used in testing.
	void SetNoResponse(bool no_response);
private:
	int port_num;
	void InsertRecordLog(const LogRecord &log_record);
	long GetPromisedTimeFromBackup();
	bool InformBackupToWriteFile(const std::string &file_name,
			const std::string &file_content);
	// Commits the logs to the file system.
	void ApplyLogs();
	void ShutDown();
	void GetHeartBeat();
	// Must be called.
	std::thread GetCommitingLogsThread();
	std::thread GetHeartBeatThread();

	// A mutex to protect log_record_list_ and last_request_time_.
	std::mutex log_record_list_mtx_;
	std::mutex cp_mutex;
	std::list<LogRecord> log_record_list_;
	long last_request_time_ = 0;

	BackupServerFrontEnd *backup_server_frontend_;
	// True if the primary server is running.
	bool running_;
	std::thread commiting_logs_thread_;
	std::thread heart_beat_thread_;
	int next_available_log_id_ = 0;
	int view_number_ = 0;
	int commit_point_ = -1;
	int log_ap = -1;
	long promised_time_ = 0;
	bool is_backup_down_ = false;
	bool is_witness_down_ = false;
	bool no_response_ = false;
	std::vector<Member> memberList;
	WitnessServerFrontEnd *witness_server_;
};

#endif
