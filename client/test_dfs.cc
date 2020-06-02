/*
 * test_dfs.cc
 *
 *  Created on: Jun 1, 2020
 *      Author: zhihua
 */

#include "../server/backup/backupFrontEnd.h"
#include "../server/primary/primaryFrontEnd.h"
#include "../server/witness/witnessFrontEnd.h"


int main(){
	BackupServerFrontEnd backup("127.0.0.1", 8070);
	WitnessServerFrontEnd witness("127.0.0.1", 8071);
	PrimaryServerFrontEnd primary("127.0.0.1", 8080);


	primary.WriteFile("test.txt", "Hello world.");


	backup.SetNoResponse(true);

	backup.WriteFile("test2.txt", "Hello abcd.");


	return 0;
}

