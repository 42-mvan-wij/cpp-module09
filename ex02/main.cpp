#include <vector>
#include <iostream>
#include <fstream>
#include "PmergeMe.hpp"

# define INSERTION_LENGTH 5

int main(int argc, char **argv) { // TODO: test with overflow (also maybe do for other exercises), and check if duplicates are working correctly (may error, doesn't have to)
	try {
		size_t start = (argc == 0) ? 0 : 1;
		PmergeMe pmerge_me = PmergeMe::from(&argv[start]);
		pmerge_me.sort();
	} catch (std::exception & e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
