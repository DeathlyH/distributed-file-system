#include "rpc.h"

//RpcServer::RpcServer(int port_number):rpc_server_(port_number) {}
//
//void RpcServer::Start() {
//  rpc_server_.bind("CreateFile", &RpcServer::CreateFile);
//  rpc_server_.bind("WriteFile", &RpcServer::WriteFile);
//  rpc_server_.bind("ReadFile", &RpcServer::ReadFile);
//  rpc_server_.run();
//}
//
//
//
//RpcClient::RpcClient(const std::string& address, int port_number): rpc_client_(address, port_number) {}
//
//bool RpcClient::CreateFile(const std::string& file_name) {
//  auto result = rpc_client_.call("CreateFile", file_name).as<bool>();
//  return result;
//}
