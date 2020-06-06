#ifndef SERVER_BACKUP_BACKUPFRONTEND_H_
#define SERVER_BACKUP_BACKUPFRONTEND_H_

#include <string>
#include "../../common/common.h"

// Definition of the front end of the backup server. It talks to the back end of the primary server.
// Uses RPC or network.
class BackupServerFrontEnd {
public:
	//explicit BackupServerFrontEnd(BackupServerBackEnd* backup_server_backend);
	BackupServerFrontEnd(std::string host_ip, int port_num);
	bool Start();
	// If true, the backup server agrees to commit this log.
	bool RequestCommit(const PayLoad &payload);
	long GetPromiseTime();
	void Commit(const PayLoad &payload);
	// Only used in testing.
	void SetNoResponse(bool no_response);

	void Demote();

	// Will be used when working as primary.
	std::string ReadFile(const std::string &file_name);
	bool WriteFile(const std::string &file_name,
			const std::string &file_content);
	std::string get_ip(){
		return host_ip;
	}

	int get_port(){
		return port_num;
	}
private:
	//BackupServerBackEnd* backup_server_backend_;
	bool no_response_ = false;
	int port_num;
	std::string host_ip;
};

#endif /* SERVER_BACKUP_BACKUPFRONTEND_H_ */
