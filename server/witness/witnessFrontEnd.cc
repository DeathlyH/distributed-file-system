#include "witnessFrontEnd.h"
#include "rpc/client.h"
/*
 * rpc client stub for WitnessServerBackEnd
 */
bool WitnessServerFrontEnd::RecordLogRecords(const PayLoad &payload) {
	rpc::client c(host_ip, port_num);
	bool result = c.call("RecordLogRecords", payload).as<bool>();
	return result;
}

bool WitnessServerFrontEnd::RequestViewChange(const PayLoad &payload) {
	rpc::client c(host_ip, port_num);
	bool result = c.call("RequestViewChange", payload).as<bool>();
	return result;
}

PayLoad WitnessServerFrontEnd::GetLogRecords() {
	rpc::client c(host_ip, port_num);
	PayLoad result = c.call("GetLogRecords").as<PayLoad>();
	return result;
}

