/*
 * witness.cc
 *
 * rpc server for witness server
 *
 */

#include <iostream>
#include "witness.h"
#include <string>
#include "rpc/server.h"


WitnessServer::WitnessServer(int port_num) :
		port_num(port_num) {
	log_record_list_.clear();

}

/*
 *  primary/ backup server ask the witness to change the view.
 *
 *  @param payload: msg for view change.
 *  @return true if view change is a success.
 */
bool WitnessServer::RequestViewChange(const PayLoad &payload) {

	std::string requester[2] = { "backup", "primary" };
	std::cout << "Witness current view no.:" << current_view_number_
			<< "\nReceived request view no. change to " << payload.view_number
			<< " from " << requester[payload.is_primary_server] << std::endl;

	//the first server reached the witness will update the view number.
	if (payload.view_number <= current_view_number_) {
		std::cout << "Denied" << std::endl;
		return false;
	}

	current_view_number_ = payload.view_number;
//	bool is_primary_server = payload.is_primary_server;
//
//	if (is_primary_server && !backup_requested_view_change_) {
//		primary_requested_view_change_ = true;
//		current_view_number_ = payload.view_number;
//		std::cout << "Succeeded" << std::endl;
//		return true;
//	}
//	if (!is_primary_server && !primary_requested_view_change_) {
//		backup_requested_view_change_ = true;
//		current_view_number_ = payload.view_number;
//		std::cout << "Succeeded" << std::endl;
//		return true;
//	}

	return true;
}

/*
 * witness server is asked to backup the operation log.
 *
 * @param payload: operation log
 * @return true if log successfully.
 */
bool WitnessServer::RecordLogRecords(const PayLoad &payload) {


	if (payload.log_record_vector.empty()) {
		std::cout << "Witness server: Receive a heartbeat. \n";
		//std::cout << ":."<<std::endl;
	} else {
		log_record_list_mtx_.lock();
		for (const auto &log : payload.log_record_vector) {
			log_record_list_.push_back(log);
			std::cout << "Witness server: Record log " << log.log_id << std::endl;
		}
		log_record_list_mtx_.unlock();
	}

	return true;
}

/*
 * send the operation log to the recovered server
 *
 * @return payload: operation logs so far.
 */
PayLoad WitnessServer::GetLogRecords() {
	std::cout << "Witness server: Recover log records" << std::endl;
	PayLoad payload;
	log_record_list_mtx_.lock();
	for (const auto &log : log_record_list_) {
		payload.log_record_vector.push_back(log);
	}
	log_record_list_mtx_.unlock();
	payload.view_number = current_view_number_;
	return payload;
}

int main() {

	WitnessServer backend(8071);

	//register service

	rpc::server srv(backend.getPortNum());
	srv.bind("RequestViewChange", [&backend](PayLoad payload) {
		return backend.RequestViewChange(payload);
	});
	srv.bind("RecordLogRecords", [&backend](PayLoad payload) {
		return backend.RecordLogRecords(payload);
	});

	srv.bind("GetLogRecords", [&backend]() {
		return backend.GetLogRecords();
	});

	srv.suppress_exceptions(true); //turn the exception to error response
	std::cout << "Witness server is running... \n";
	srv.run();

	return 0;
}

