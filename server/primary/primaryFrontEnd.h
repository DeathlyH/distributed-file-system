/*
 * primaryFrontEnd.h
 *
 *  Created on: Jun 1, 2020
 *      Author: zhihua
 */

#ifndef SERVER_PRIMARY_PRIMARYFRONTEND_H_
#define SERVER_PRIMARY_PRIMARYFRONTEND_H_

#include <string>

// Definition of the front end of the primary server. It receives the requests from the client, and
// forwards them to the backend server. After the backend finishes processing the requests, the front
// end returns them back to the client.
// Some possible ways of communication are RPC or network.
class PrimaryServerFrontEnd {
public:
	//explicit PrimaryServerFrontEnd(PrimaryServerBackEnd* primary_server_backend);
	PrimaryServerFrontEnd(std::string host_ip, int port_num);
	bool Start();
	std::string ReadFile(const std::string &file_name);
	bool WriteFile(const std::string &file_name,
			const std::string &file_content);
	void BringUpBackUp();

private:
	//PrimaryServerBackEnd* primary_server_backend_;
	std::string host_ip;
	int port_num;

};



#endif /* SERVER_PRIMARY_PRIMARYFRONTEND_H_ */
