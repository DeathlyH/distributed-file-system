/*
 * Test dfs read/write
 */
#include <iostream>
#include "../server/backup/backupFrontEnd.h"
#include "../server/primary/primaryFrontEnd.h"
#include "../server/witness/witnessFrontEnd.h"

int main(int argc, char *argv[]) {

	if (argc != 2) {
		std::cout << "Usage: ./test_dfs <1 = primary, 0 = backup>" << std::endl;
		return 0;
	}
	BackupServerFrontEnd backup("127.0.0.1", 8070);
	WitnessServerFrontEnd witness("127.0.0.1", 8071);
	PrimaryServerFrontEnd primary("127.0.0.1", 8072);


	for (int i = 0; i < 100; i++) {

		if (std::stoi(argv[1]) == 0) {

			backup.WriteFile("test.txt", "Hello world.");

			std::string file_content = backup.ReadFile("test.txt");

			std::cout << "Read back test.txt" << std::endl;

			std::cout << file_content << std::endl;
		} else if (std::stoi(argv[1]) == 1) {
			primary.WriteFile("test.txt", "Hello world.");

			std::string file_content = primary.ReadFile("test.txt");

			std::cout << "Read back test.txt" << std::endl;

			std::cout << file_content << std::endl;
		}

	}

//backup.SetNoResponse(true);

//backup.WriteFile("test2.txt", "Hello abcd.");

	return 0;
}

