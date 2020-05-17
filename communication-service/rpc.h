#ifndef rpc
#define rpc

//#include "communication-service-interface.h"
//
//class RpcServer : public CommunicationServiceServerInterface {
//public:
//  RpcService(int port_number);
//  bool CreateFile(const std::string& file_name) override {}
//  bool WriteFile(const std::string& file_name, const std::string& file_content) override {}
//  std::string ReadFile(const std::string& file_name) override {}
//  
//  void Start();
//private:
//  rpc::server rpc_server_;
//};
//
//class RpcClient : public CommunicationServiceClientInterface {
//public:
//  RpcClient(const std::string& address, int port_number);
//  bool CreateFile(const std::string& file_name) override;
//  bool WriteFile(const std::string& file_name, const std::string& file_content) override;
//  std::string ReadFile(const std::string& file_name) override;
//private:
//  rpc::client rpc_client_;
//}

#endif
