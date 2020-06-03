/*
 * backupFrontEnd.cc
 *
 */

#include <iostream>
#include "backupFrontEnd.h"
#include "rpc/client.h"

//BackupServerFrontEnd::BackupServerFrontEnd(BackupServerBackEnd* backup_server_backend): backup_server_backend_(backup_server_backend) {}

BackupServerFrontEnd::BackupServerFrontEnd(std::string host_ip, int port_num) {
	this->host_ip = host_ip;
	this->port_num = port_num;
}
bool BackupServerFrontEnd::Start() {
	std::cout << "BackupServerFrontEnd started. \n";
	return true;
}

long BackupServerFrontEnd::GetPromiseTime() {
	std::cout << "primary server calls GetPromiseTime. \n";
	rpc::client c(host_ip, port_num);
	try {
		long tmp = c.call("GetPromiseTime").as<long>();
		return tmp;
	} catch (...) {
		std::cout << "some exception happened during GetPromiseTime\n";
		return 0;
	}
	//return backup_server_backend_->GetPromiseTime();
}

bool BackupServerFrontEnd::RequestCommit(const PayLoad &payload) {
	if (no_response_) {
		return false;
	}
	std::cout << "primary server calls RequestCommit(). \n";
	rpc::client c(host_ip, port_num);
	bool result = c.call("RequestCommit", payload).as<bool>();
	return result;

	//return backup_server_backend_->RequestCommit(payload);
}

void BackupServerFrontEnd::Commit(const PayLoad &payload) {
	std::cout << "primary server calls Commit(). \n";
	rpc::client c(host_ip, port_num);
	c.async_call("Commit", payload);

	//backup_server_backend_->Commit(payload);
}

void BackupServerFrontEnd::SetNoResponse(bool no_response) {
	no_response_ = no_response;
}

void BackupServerFrontEnd::Demote() {
	rpc::client c(host_ip, port_num);
	c.call("Demote");
	//backup_server_backend_->Demote();
}

std::string BackupServerFrontEnd::ReadFile(const std::string &file_name) {
	rpc::client c(host_ip, port_num);
	std::string content = c.call("ReadFile", file_name).as<std::string>();
	return content;
}

bool BackupServerFrontEnd::WriteFile(const std::string &file_name,
		const std::string &file_content) {
	rpc::client c(host_ip, port_num);
	bool result = c.call("WriteFile", file_name, file_content).as<bool>();
	return result;
}


