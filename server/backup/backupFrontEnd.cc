/*
 * backupFrontEnd.cc
 *
 * rpc client for the backup server
 *
 */

#include <iostream>
#include <exception>
#include "backupFrontEnd.h"
#include "rpc/client.h"
#include "rpc/rpc_error.h"

BackupServerFrontEnd::BackupServerFrontEnd(std::string host_ip, int port_num) {
	this->host_ip = host_ip;
	this->port_num = port_num;
}

long BackupServerFrontEnd::GetPromiseTime() {
	//std::cout << "primary server calls GetPromiseTime. \n";
	rpc::client c(host_ip, port_num);
	try {
		long tmp = c.call("GetPromiseTime").as<long>();
		return tmp;
	} catch (...) {
		std::cout << "some exception happened during GetPromiseTime\n";
		return 0;
	}
}

bool BackupServerFrontEnd::RequestCommit(const PayLoad &payload) {
	if (no_response_) {
		return false;
	}
	rpc::client c(host_ip, port_num);
	//set 500 milliseconds timeout.
	c.set_timeout(5000);
	try {
		bool result = c.call("RequestCommit", payload).as<bool>();

		return result;
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return false;
}

/*
 * Asynchronous commit rpc call
 */
void BackupServerFrontEnd::Commit(const PayLoad &payload) {
	std::cout<<"backup commit called"<<std::endl;
	rpc::client c(host_ip, port_num);
	c.call("Commit", payload);
}

void BackupServerFrontEnd::SetNoResponse(bool no_response) {
	no_response_ = no_response;
}

void BackupServerFrontEnd::Demote() {
	rpc::client c(host_ip, port_num);
	c.set_timeout(500);
	try {
		c.call("Demote");
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return;
}

std::string BackupServerFrontEnd::ReadFile(const std::string &file_name) {
	std::cout << "Client: Call Backup WriteFile(" << file_name << "). \n";
	rpc::client c(host_ip, port_num);
	std::string content = c.call("ReadFile", file_name).as<std::string>();
	return content;
}

bool BackupServerFrontEnd::WriteFile(const std::string &file_name,
		const std::string &file_content) {
	std::cout << "Client: Call Backup WriteFile(" << file_name << "). \n";
	rpc::client c(host_ip, port_num);
	bool result = c.call("WriteFile", file_name, file_content).as<bool>();
	return result;
}

