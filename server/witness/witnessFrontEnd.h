

#ifndef SERVER_WITNESS_WITNESSFRONTEND_H_
#define SERVER_WITNESS_WITNESSFRONTEND_H_

#include <string>
#include "../../common/common.h"


/*
 * rpc client stub for WitnessServerBackEnd
 */
class WitnessServerFrontEnd {
public:
	WitnessServerFrontEnd(std::string host_ip, int port_num) :host_ip(host_ip), port_num(port_num){}
	bool RecordLogRecords(const PayLoad &payload);
	bool RequestViewChange(const PayLoad &payload);

	PayLoad GetLogRecords();

	std::string get_ip(){
		return host_ip;
	}

	int get_port(){
		return port_num;
	}

private:
	std::string host_ip;
	int port_num;

};




#endif /* SERVER_WITNESS_WITNESSFRONTEND_H_ */
