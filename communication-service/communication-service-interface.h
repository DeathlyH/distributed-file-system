#ifndef communication_service_interface
#define communication_service_interface

#include <string>

class CommunicationServiceServerInterface {
public:
  CommunicationServiceServerInterface() = default;
  virtual bool CreateFile(const std::string& file_name) = 0;
  virtual bool WriteFile(const std::string& file_name, const std::string& file_content) = 0;
  virtual std::string ReadFile(const std::string& file_name) = 0;
};

class CommunicationServiceClientInterface {
public:
  CommunicationServiceClientInterface() = default;
  virtual bool CreateFile(const std::string& file_name) = 0;
  virtual bool WriteFile(const std::string& file_name, const std::string& file_content) = 0;
  virtual std::string ReadFile(const std::string& file_name) = 0;
};

#endif
