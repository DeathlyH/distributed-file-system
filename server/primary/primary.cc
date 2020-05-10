#include "primary.h"

void PrimaryServer::CreateFile(const std::string& file_name) {
  std::ofstream file(file_name);
  file.close();
}

void PrimaryServer::WriteFile(const std::string& file_name, const std::string& file_content) {
  std::ofstream file(file_name);
  file << file_content;
  file.close();
}
