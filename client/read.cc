/*
 * Test dfs read/write
 */
#include <iostream>
#include <string>
#include "../server/backup/backupFrontEnd.h"
#include "../server/primary/primaryFrontEnd.h"
#include "../server/witness/witnessFrontEnd.h"

int usage() {
	std::cout
			<< "Usage: ./read_harp <1 = primary, 0 = backup> <filename>"
			<< std::endl;
	return 0;
}

int main(int argc, char *argv[]) {

	if (argc != 3) {
		usage();
		return -1;
	}
	BackupServerFrontEnd backup("127.0.0.1", 8070);
	WitnessServerFrontEnd witness("127.0.0.1", 8071);
	PrimaryServerFrontEnd primary("127.0.0.1", 8072);


	std::string filename(argv[2]);

	std::cout<<filename<<std::endl;

	if (std::stoi(argv[1]) == 0) {



		std::string file_content = backup.ReadFile(filename);

		//std::cout << "Read back test.txt" << std::endl;

		std::cout << file_content << std::endl;
	} else if (std::stoi(argv[1]) == 1) {


		std::string file_content = primary.ReadFile(filename);

		std::cout << file_content << std::endl;
	} else {
		usage();
		return -1;
	}

	return 0;
}

