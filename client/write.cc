/*
 * Test dfs read/write
 */
#include <iostream>
#include <string>
#include "../server/backup/backupFrontEnd.h"
#include "../server/primary/primaryFrontEnd.h"
#include "../server/witness/witnessFrontEnd.h"

int usage(){
	std::cout << "Usage: ./write_harp <1 = primary, 0 = backup> <filename> <file_content>" << std::endl;
			return 0;
}

int main(int argc, char *argv[]) {

	if (argc != 4) {
		usage();
		return -1;
	}
	BackupServerFrontEnd backup("127.0.0.1", 8070);
	WitnessServerFrontEnd witness("127.0.0.1", 8071);
	PrimaryServerFrontEnd primary("127.0.0.1", 8072);

	std::string filename(argv[2]);
	std::string file_content(argv[3]);





		if (std::stoi(argv[1]) == 0) {

			backup.WriteFile(filename, file_content);

		} else if (std::stoi(argv[1]) == 1) {
			primary.WriteFile(filename, file_content);


		}
		else{
			usage();
			return -1;
		}



	return 0;
}

