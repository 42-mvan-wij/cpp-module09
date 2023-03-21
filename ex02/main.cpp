#include <vector>
#include <iostream>
#include <fstream>
#include "PmergeMe.hpp"

# define INSERTION_LENGTH 5

template<typename T>
using container1 = std::vector<T>; // TODO: hardcode into algorithm

namespace my_random {
	template<typename T>
	T rand() {
		static std::ifstream random;
		if (!random.is_open()) {
			random.open("/dev/random");
			if (random.fail()) {
				throw "ERROR";
			}
		}
		char bytes[sizeof(T)];
		random.read(bytes, sizeof(bytes));
		T value;
		char *v_bytes = (char *)&value;
		for (int byte = 0; byte < sizeof(T); byte++) {
			v_bytes[byte] = 0;
		}
		for (int bit = 0; bit < 8; bit++) {
			for (int byte = 0; byte < sizeof(T); byte++) {
				char v = (bytes[byte] >> bit) & 1;

				int v_bit = bit * sizeof(T) + byte;
				int v_byte = v_bit / 8;
				int v_byte_bit = v_bit - v_byte * 8;

				v_bytes[v_byte] = v_bytes[v_byte] | (v << v_byte_bit);
			}
		}
		return value;
	}
}

int binary_search(container1<unsigned int> & array_to_sort, int value, int search_start, int search_end) {
	while (true) {
		int index = (search_start + search_end) / 2;
		if (value == array_to_sort[index]) {
			return (index);
		}
		if (value < array_to_sort[index]) {
			if (index == search_start) {
				return (index);
			}
			search_end = index - 1;
		}
		if (value > array_to_sort[index]) {
			if (index == search_end) {
				return (index + 1);
			}
			search_start = index + 1;
		}
	}
}

void insertion_sort(container1<unsigned int> & array_to_sort, int min_index, int max_index) {
	for (int current_index = min_index + 1; current_index <= max_index; current_index++) {
		unsigned int value = array_to_sort[current_index];
		int index = binary_search(array_to_sort, value, min_index, current_index - 1);
		array_to_sort.erase(array_to_sort.begin() + current_index);
		array_to_sort.insert(array_to_sort.begin() + index, value);
	}
}

void merge(container1<unsigned int> & array_to_sort, int min_index, int mid_index, int max_index) {
	// no numbers remaining right && left
	while (mid_index != max_index && min_index != mid_index + 1) {
		if (array_to_sort[min_index] <= array_to_sort[mid_index + 1]) {
			min_index++;
		}
		else {
			// copy value at mid_index + 1
			unsigned int value = array_to_sort[mid_index + 1];
			// remove value at mid_index + 1
			array_to_sort.erase(array_to_sort.begin() + mid_index + 1);
			// insert copied value between min_index - 1 and min_index
			array_to_sort.insert(array_to_sort.begin() + min_index, value);
			mid_index++;
			min_index++;
		}
	}
}

// [4, 5 | 3, 4.5] min = 0, mid = 1, max = 3
// [3, 4, 5 | 4.5] min = 1, mid = 2, max = 3
// [3, 4, 5 | 4.5] min = 2, mid = 2, max = 3
// [3, 4, 4.5, 5 | ] min = 3, mid = 3, max = 3

// [4, 5 | 2, 3] min = 0, mid = 1, max = 3
// [2, 4, 5 | 3] min = 1, mid = 2, max = 3
// [2, 3, 4, 5 | ] min = 2, mid = 3, max = 3

void merge_insert_sort(container1<unsigned int> & array_to_sort, int min_index, int max_index) {
	int length = max_index - min_index + 1;
	if (length > INSERTION_LENGTH + 1) {
		int mid_index = (min_index + max_index) / 2;
		merge_insert_sort(array_to_sort, min_index, mid_index);
		merge_insert_sort(array_to_sort, mid_index + 1, max_index);
		merge(array_to_sort, min_index, mid_index, max_index);
	}
	else {
		insertion_sort(array_to_sort, min_index, max_index);
	}
}

enum {
	BASE_SEED,
	TIME_SEED,
	CRYPT_SEED,
	CRYPT
};

int main(int argc, char **argv) { // TODO: test with over-/underflow (also maybe do for other exercises), and check if duplicates are working correctly (may error, doesn't have to
	int (*rand_fn)() = std::rand;

	switch (CRYPT) {
		case BASE_SEED:
			rand_fn = std::rand;
			break;
		case TIME_SEED:
			std::srand(std::time(NULL));
			rand_fn = std::rand;
			break;
		case CRYPT_SEED:
			try {
				std::srand(my_random::rand<unsigned int>());
			} catch (std::exception & e) {
				return EXIT_FAILURE;
			}
			rand_fn = std::rand;
			break;
		case CRYPT:
			rand_fn = my_random::rand<int>;
			break ;
	}


	{
		size_t start = (argc == 0) ? 0 : 1;
		PmergeMe pmerge_me = PmergeMe::from(&argv[start]);
		std::cout << pmerge_me << std::endl;
		pmerge_me.sort();
		std::cout << std::endl;
		std::cout << pmerge_me << std::endl;
	}
	{
		container1<unsigned int> v;
		v.push_back(4);
		v.push_back(5);
		v.push_back(3);
		v.push_back(8);

		merge(v, 0, (v.size() - 1) / 2, v.size() - 1);
		for (decltype(v)::iterator it = v.begin(); it + 1 != v.end(); it++) {
			assert(*it <= *(it + 1));
		}
	}
	{
		container1<unsigned int> v;
		v.push_back(4);
		v.push_back(6);
		v.push_back(3);
		v.push_back(5);

		merge(v, 0, (v.size() - 1) / 2, v.size() - 1);
		for (decltype(v)::iterator it = v.begin(); it + 1 != v.end(); it++) {
			assert(*it <= *(it + 1));
		}
	}
	{
		container1<unsigned int> v;
		v.push_back(5);
		v.push_back(4);
		v.push_back(6);
		v.push_back(3);

		merge_insert_sort(v, 0, v.size() - 1);
		for (decltype(v)::iterator it = v.begin(); it + 1 != v.end(); it++) {
			assert(*it <= *(it + 1));
		}
	}
	{
		container1<unsigned int> v;
		for (int i = 0; i < 100; ++i) {
			v.push_back(rand_fn());
		}

		merge_insert_sort(v, 0, v.size() - 1);
		for (decltype(v)::iterator it = v.begin(); it + 1 != v.end(); it++) {
			assert(*it <= *(it + 1));
		}
	}
	std::cout << "yay" << std::endl;
	return (EXIT_SUCCESS);
}
