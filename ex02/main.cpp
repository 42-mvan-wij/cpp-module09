#include <vector>
#include <iostream>
#include <fstream>
#include "PmergeMe.hpp"

# define INSERTION_LENGTH 5

int main(int argc, char **argv) {
	if (argc <= 1) {
		std::cerr << "Usage: ./PMergeMe <numbers>" << std::endl;
		return (EXIT_FAILURE);
	}
	try {
		PmergeMe pmerge_me = PmergeMe::from(&argv[1]);
		pmerge_me.sort();
	} catch (std::exception & e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
