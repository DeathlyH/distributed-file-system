/*
 * primaryFrontEnd.cc
 *
 */

#include <iostream>
#include "primaryFrontEnd.h"
#include "rpc/client.h"


//PrimaryServerFrontEnd::PrimaryServerFrontEnd(PrimaryServerBackEnd* primary_server_backend):primary_server_backend_(primary_server_backend) {}
PrimaryServerFrontEnd::PrimaryServerFrontEnd(std::string host_ip,
		int port_num) {
	this->host_ip = host_ip;
	this->port_num = port_num;
}

bool PrimaryServerFrontEnd::Start() {
	// Set up rpc or network connections.
	std::cout << "PrimaryServerFrontEnd Started.\n";
	return true;
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
	return result;
}

void PrimaryServerFrontEnd::BringUpBackUp() {
	std::cout << "Bring up backup" << std::endl;
	rpc::client c(host_ip, port_num);
	c.call("BringUpBackUp");

}
