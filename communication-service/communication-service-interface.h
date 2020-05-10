#ifndef communication_service_interface
#define communication_service_interface

#include <string>

class CommunicationServiceInterface {
public:
  CommunicationServiceInterface() = default;
  virtual void CreateFile() = 0;
  virtual void WriteFile(const std::string& file_name, const std::string& file_content) = 0;
  virtual void ReadFile() = 0;
};

#endif
