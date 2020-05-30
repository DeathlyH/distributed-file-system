//#include <iostream>
//#include "primary/primary.h"
//#include "backup/backup.h"
//#include "witness/witness.h"
//
//int main() {
//  // Initialize witness server.
//  WitnessServer witness_server;
//
//  
//  // Initialize backup server.
//  BackupServerBackEnd backup_server_backend;
//  BackupServerFrontEnd backup_server_frontend(&backup_server_backend);
//  backup_server_backend.SetWitnessServer(&witness_server);
//  backup_server_frontend.Start();
//
//  // Initialize primary server.
//  PrimaryServerBackEnd back_end;
//  back_end.SetBackupServerFrontEnd(&backup_server_frontend);
//  back_end.SetWitnessServer(&witness_server);
//  PrimaryServerFrontEnd front_end(&back_end);
//  front_end.Start();
//
//  std::this_thread::sleep_for (std::chrono::seconds(4));
//  backup_server_frontend.SetNoResponse(true);
//  std::cout << "No reponse is set to True. \n";
//
//  // Wait for everything to be committed.
//  std::this_thread::sleep_for (std::chrono::seconds(15));
//  std::cout << "The End. \n";
//  return 0;
//}
