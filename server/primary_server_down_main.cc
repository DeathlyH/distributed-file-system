#include <iostream>
#include "primary/primary.h"
#include "backup/backup.h"
#include "witness/witness.h"

int main() {
  // Initialize witness server.
  WitnessServer witness_server;

  // Initialize backup server.
  BackupServerBackEnd backup_server_backend;
  BackupServerFrontEnd backup_server_frontend(&backup_server_backend);
  backup_server_backend.SetWitnessServer(&witness_server);
  backup_server_backend.Start();
  backup_server_frontend.Start();

  // Initialize primary server.
  PrimaryServerBackEnd back_end;
  back_end.SetBackupServerFrontEnd(&backup_server_frontend);
  back_end.SetWitnessServer(&witness_server);
  back_end.Start();
  PrimaryServerFrontEnd front_end(&back_end);
  front_end.Start();

  std::this_thread::sleep_for (std::chrono::seconds(4));
  back_end.SetNoResponse(true);
  std::cout << "No reponse is set to True. \n";

  // Wait for everything to be committed.
  std::this_thread::sleep_for (std::chrono::seconds(15));
  std::string file_name = "/Users/Jiaming/Desktop/test/file.txt";
  std::string file_content = "hello world 14:57 05/30";
  backup_server_frontend.WriteFile(file_name, file_content);
  
  std::this_thread::sleep_for (std::chrono::seconds(10));
  std::cout << "The End. \n";
  return 0;
}
