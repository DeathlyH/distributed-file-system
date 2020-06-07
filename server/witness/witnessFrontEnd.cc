/*
 * witnessFrontEnd.cc
 *
 * rpc client for witness server
 *
 */

#include "witnessFrontEnd.h"
#include "rpc/client.h"
#include <iostream>

bool WitnessServerFrontEnd::RecordLogRecords(const PayLoad &payload) {
	//std::cout << "request witness server to record log" << std::endl;
	rpc::client c(host_ip, port_num);
	bool result = c.call("RecordLogRecords", payload).as<bool>();
	return result;
}

bool WitnessServerFrontEnd::RequestViewChange(const PayLoad &payload) {
	std::cout << "\trequest view change for witness" << std::endl;
	rpc::client c(host_ip, port_num);
	bool result = c.call("RequestViewChange", payload).as<bool>();
	return result;
}

PayLoad WitnessServerFrontEnd::GetLogRecords() {
	std::cout << "\trequest get logs from the witness" << std::endl;
	rpc::client c(host_ip, port_num);
	PayLoad result = c.call("GetLogRecords").as<PayLoad>();
	return result;
}

