/*
 * primary_main.cpp
 *
 *  Created on: May 23, 2020
 *      Author: zhihua
 */

#include <rpc/server.h>
#include <iostream>

//#include "../backup/backup.h"
#include "primary.h"

int main() {
	// Initialize backup server.
	BackupServerBackEnd backup_server_backend;
	BackupServerFrontEnd backup_Server_frontend(&backup_server_backend);
	backup_Server_frontend.Start();
	// Initialize primary server.
	PrimaryServerBackEnd back_end;
	back_end.SetBackupServerFrontEnd(&backup_Server_frontend);
	PrimaryServerFrontEnd front_end(&back_end);
	front_end.Start();

	rpc::server srv(8080);
	srv.bind("WriteFile",
			[&front_end](const std::string &file_name,
					const std::string &file_content) {
				front_end.WriteFile(file_name, file_content);
			});
//	srv.bind("ReadFile", &PrimaryServerFrontEnd::ReadFile);
//	srv.bind("WriteFile", &PrimaryServerFrontEnd::WriteFile);
//	srv.bind("CreateFile", &PrimaryServerFrontEnd::CreateFile);
//	srv.bind("DeleteFile", &PrimaryServerFrontEnd::DeleteFile);
	srv.suppress_exceptions(true); //turn the exception to error response
	srv.run();
}
