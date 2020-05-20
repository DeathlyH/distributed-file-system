#include "../../common/common.h"

class WitnessServer {
public:
  WitnessServer();
  PayLoad GetLogRecords();
  bool RecordLogRecords(const PayLoad& payload);
};
