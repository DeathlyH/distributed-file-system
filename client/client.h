#ifndef client
#define client

#include "../communication-service/communication-service-interface.h"

class Client {
public:
  explicit Client(CommunicationServiceInterface* communication_service_pointer);

  CommunicationServiceInterface* GetCommunicationService();

private:
  CommunicationServiceInterface* communication_service_pointer_;
};

#endif
