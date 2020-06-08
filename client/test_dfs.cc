/*
 * Test dfs read/write
 */
#include <iostream>
#include <string>
#include "../server/backup/backupFrontEnd.h"
#include "../server/primary/primaryFrontEnd.h"
#include "../server/witness/witnessFrontEnd.h"

int main(int argc, char *argv[]) {

	if (argc != 3) {
		std::cout << "Usage: ./test_dfs <1 = primary, 0 = backup> <repeats>" << std::endl;
		return 0;
	}
	BackupServerFrontEnd backup("127.0.0.1", 8070);
	WitnessServerFrontEnd witness("127.0.0.1", 8071);
	PrimaryServerFrontEnd primary("127.0.0.1", 8072);

    int repeats = std::stoi(argv[2]);
	for (int i = 0; i < repeats; i++) {



		std::string filename = "test.txt";

		if (std::stoi(argv[1]) == 0) {

			backup.WriteFile(filename, "Hello world.");

			std::string file_content = backup.ReadFile(filename);

			//std::cout << "Read back test.txt" << std::endl;

			std::cout << "Client: Read File Content: "<<file_content << std::endl;
		} else if (std::stoi(argv[1]) == 1) {
			primary.WriteFile(filename, "Hello world.");

			std::string file_content = primary.ReadFile(filename);

			std::cout << "Client: Read File Content: "<<file_content << std::endl;
		}

	}

//backup.SetNoResponse(true);

//backup.WriteFile("test2.txt", "Hello abcd.");

	return 0;
}

