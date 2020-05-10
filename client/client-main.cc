#include <iostream>
#include "client.h"
#include "../communication-service/rpc.h"

int main() {
  RpcService rpc_service(8000);
  CommunicationServiceInterface* pointer = &rpc_service;
  Client user_client{pointer};
  user_client.GetCommunicationService()->WriteFile("file.txt", "hello world");
  return 0;
}
