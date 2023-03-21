#include <cstdlib>
#include <iostream>
#include <fstream>
#include "BitcoinExchange.hpp"

void check_leaks() {
	std::cout << std::endl;
	std::system("leaks -q btc");
}

int main(int argc, char **argv) {
	std::atexit(&check_leaks);
	std::cout << std::boolalpha;

	if (argc != 2) {
		std::cout << "Usage: btc <filename>" << std::endl;
		return (EXIT_FAILURE);
	}

	std::ifstream file(argv[1]);
	if (file.fail()) {
		std::cout << "Error: could not open file: `" << argv[1] << "`" << std::endl;
		return (EXIT_FAILURE);
	}

	try {
		BitcoinExchange btc("data.csv");
		btc.parse(file);
	} catch (std::exception & e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
