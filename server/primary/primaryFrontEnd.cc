/*
 * primaryFrontEnd.cc
 *
 * rpc client for primary server
 *
 */

#include <iostream>
#include "primaryFrontEnd.h"
#include "rpc/client.h"

PrimaryServerFrontEnd::PrimaryServerFrontEnd(std::string host_ip,
		int port_num) {
	this->host_ip = host_ip;
	this->port_num = port_num;
}

bool PrimaryServerFrontEnd::WriteFile(const std::string &file_name,
		const std::string &file_content) {
	std::cout << "Client calls WriteFile() " << file_name << ". \n";
	rpc::client c(host_ip, port_num);
	bool result = c.call("WriteFile", file_name, file_content).as<bool>();
	return result;
}

std::string PrimaryServerFrontEnd::ReadFile(const std::string &file_name) {
	std::cout << "Client calls ReadFile() " << file_name << ". \n";
	rpc::client c(host_ip, port_num);
	std::string result = c.call("ReadFile", file_name).as<std::string>();
	std::cout << result << std::endl;
	return result;
}

/*
 * backup server will ask primary if he can join his group.
 */
bool PrimaryServerFrontEnd::BringUpBackUp() {
	std::cout << "Bring up backup server" << std::endl;
	rpc::client c(host_ip, port_num);
	c.set_timeout(500);
	try {
		bool result = c.call("BringUpBackUp").as<bool>();
		std::cout << "Succeeded." << std::endl;
		return result;
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	};
	return false;

}
