#include<iostream>
#include<string>
#include"CPost.h"
#include"CMarket.h"

int main(int argc, char* argv[]) {
	// In case of no command line argument, let user know and exit.
	if (argc < 2) {
		std::cout << "NO COMMAND LINE ARGUMENT USED\n";
		exit(0);
	}

	// Otherwise, continue as normal.
	std::string command(argv[1]);

	// Base project implemented.
	if (command == "base") {

		// Object creation.
		Market market;
		std::string input;

		// Main program loop, will continuously accept new user input.
		while (true) {
			std::getline(std::cin, input); // Receive user input.
			if (input == "EXIT") { exit(0); } // Exit if requested.
			else { market.process_input(input); } // Otherwise, let the market process the input.
			std::cout << std::endl;
		}
	}

	// Extensions not implemented.
	else if (command[3] == '1') { exit(1); }
	else if (command[3] == '2') { exit(2); }
	return 0;
}