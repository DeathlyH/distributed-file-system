#include "client.h"

Client::Client(CommunicationServiceInterface* communication_service_pointer) :
  communication_service_pointer_(communication_service_pointer) {}

CommunicationServiceInterface* Client::GetCommunicationService() {
  return communication_service_pointer_;
}
