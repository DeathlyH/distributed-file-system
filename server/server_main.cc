#include <iostream>
#include "primary/primary.h"
#include "backup/backup.h"

//int main() {
//  // Initialize backup server.
//  BackupServerBackEnd backup_server_backend;
//  BackupServerFrontEnd backup_Server_frontend(&backup_server_backend);
//  backup_Server_frontend.Start();
//
//  // Initialize primary server.
//  PrimaryServerBackEnd back_end;
//  back_end.SetBackupServerFrontEnd(&backup_Server_frontend);
//  PrimaryServerFrontEnd front_end(&back_end);
//  front_end.Start();
//  
//  // Simulate client requests.
//  std::string file_name = "/Users/Jiaming/Desktop/test/file.txt";
//  std::string file_content = "hello world 10:57 05/30";
//  front_end.WriteFile(file_name, file_content);
//  // std::cout << front_end.ReadFile(file_name) << " \n";
//  
//  // Wait for everything to be committed.
//  std::this_thread::sleep_for (std::chrono::seconds(15));
//  std::cout << "hello\n";
//  return 0;
//}
