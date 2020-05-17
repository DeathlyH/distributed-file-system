#include "client.h"

Client::Client(CommunicationServiceClientInterface* communication_service_pointer) :
  communication_service_pointer_(communication_service_pointer) {}

CommunicationServiceClientInterface* Client::GetCommunicationService() {
  return communication_service_pointer_;
}
