#ifndef rpc
#define rpc
#include "communication-service-interface.h"

class RpcService : public CommunicationServiceInterface {
public:
  RpcService(int port_number);
  void CreateFile() override {}
  void WriteFile(const std::string& file_name, const std::string& file_content) override {}
  void ReadFile() override {}
private:
  int port_number_;
};

#endif
