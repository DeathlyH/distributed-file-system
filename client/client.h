#ifndef client
#define client

#include "../communication-service/communication-service-interface.h"

class Client {
public:
  explicit Client(CommunicationServiceClientInterface* communication_service_pointer);

  CommunicationServiceClientInterface* GetCommunicationService();

private:
  CommunicationServiceClientInterface* communication_service_pointer_;
};

#endif
