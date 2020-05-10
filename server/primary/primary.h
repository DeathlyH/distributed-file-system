#ifndef primary
#define primary

#include <iostream>
#include <fstream>
#include <string.h>

class PrimaryServer {
public:
  void CreateFile(const std::string& file_name);

  void WriteFile(const std::string& file_name, const std::string& file_content);
};

#endif
