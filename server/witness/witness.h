#ifndef witness
#define witness

#include "../../common/common.h"
#include <list>
#include <mutex>

class WitnessServer {
public:
  WitnessServer(int port_num);
  PayLoad GetLogRecords();
  void Demote();
  bool RecordLogRecords(const PayLoad& payload);
  int getPortNum(){
	  return port_num;
  }
  bool RequestViewChange(const PayLoad& payload);
  
private:
  int  port_num;
  bool primary_requested_view_change_ = false;
  bool backup_requested_view_change_ = false;
  
  std::mutex log_record_list_mtx_;
  std::list<LogRecord> log_record_list_;
  int current_view_number_ = 0;
};

#endif
