#ifndef witness
#define witness

#include "../../common/common.h"
#include <list>
#include <mutex>

class WitnessServer {
public:
  WitnessServer();
  PayLoad GetLogRecords();
  bool RecordLogRecords(const PayLoad& payload);
  
  bool RequestViewChange(const PayLoad& payload);
  
private:
  bool primary_requested_view_change_ = false;
  bool backup_requested_view_change_ = false;
  
  std::mutex log_record_list_mtx_;
  std::list<LogRecord> log_record_list_;
};

#endif
