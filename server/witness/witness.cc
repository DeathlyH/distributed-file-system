#include <iostream>
#include "witness.h"
#include "rpc/server.h"

WitnessServer::WitnessServer(int port_num): port_num(port_num) {
	LogRecord log(GetCurrentTimestamp(), 10, "WriteFile",
			"/Users/Jiaming/Desktop/test/file3.txt", "witness file logs",
			"witness");
	log_record_list_.push_back(log);
}

/*
 *
 */
bool WitnessServer::RequestViewChange(const PayLoad &payload) {
	if (payload.view_number < current_view_number_) {
		return false;
	}
	bool is_primary_server = payload.is_primary_server;

	if (is_primary_server && !backup_requested_view_change_) {
		primary_requested_view_change_ = true;
		current_view_number_ = payload.view_number;
		return true;
	}
	if (!is_primary_server && !primary_requested_view_change_) {
		backup_requested_view_change_ = true;
		current_view_number_ = payload.view_number;
		return true;
	}
	return false;
}


bool WitnessServer::RecordLogRecords(const PayLoad &payload) {
	log_record_list_mtx_.lock();
	if (payload.log_record_vector.empty()) {
		std::cout << "witness receives a heart beat. \n";
	}
	for (const auto &log : payload.log_record_vector) {
		log_record_list_.push_back(log);
	}
	log_record_list_mtx_.unlock();
	return true;
}

PayLoad WitnessServer::GetLogRecords() {
	PayLoad payload;
	log_record_list_mtx_.lock();
	for (const auto &log : log_record_list_) {
		payload.log_record_vector.push_back(log);
	}
	log_record_list_mtx_.unlock();
	return payload;
}

int main() {

	WitnessServer backend(8071);
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
	std::cout << "Witness started. \n";
	srv.run();

	return 0;
}




