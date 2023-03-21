#include <cstdlib>
#include <iostream>
#include "RPN.hpp"

void check_leaks() {
	std::cout << std::endl;
	std::system("leaks -q RPN");
}

int main(int argc, char **argv) {
	std::atexit(&check_leaks);
	std::cout << std::boolalpha;

	if (argc != 2) {
		std::cout << "Usage: RPN <expression in reverse polish notation>" << std::endl;
		return (EXIT_FAILURE);
	}

	try {
		RPN rpn(argv[1]);
		std::cout << rpn.calculate() << std::endl;
	} catch (std::exception & e) {
		std::cout << "Error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
