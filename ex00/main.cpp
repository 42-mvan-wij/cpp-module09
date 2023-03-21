#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "BitcoinExchange.hpp"

void check_leaks() {
	std::cout << std::endl;
	std::system("leaks -q btc");
}

#include "Date.hpp"
std::map<Date, int>::const_iterator binary_search_map(std::map<Date, int> & map, Date const & search_value) {
	size_t len = map.size();
	size_t search_start = 0;
	size_t search_end = len - 1;
	while (true) {
		size_t index = (search_start + search_end) / 2;
		std::map<Date, int>::const_iterator current_item = map.begin();
		advance(current_item, index);
		if (search_value == current_item->first) {
			return current_item;
		}
		else if (map.key_comp()(search_value, current_item->first)) {
			if (index == search_start) {
				return current_item;
			}
			search_end = index - 1;
		}
		else {
			if (index == search_end) {
				return (++current_item);
			}
			search_start = index + 1;
		}
	}
}

std::map<Date, int>::const_reverse_iterator binary_search_map_reverse(std::map<Date, int> & map, Date const & search_value) {
	size_t len = map.size();
	size_t search_start = 0;
	size_t search_end = len - 1;
	while (true) {
		size_t index = (search_start + search_end) / 2;
		std::map<Date, int>::const_reverse_iterator current_item = map.rbegin();
		advance(current_item, index);
		if (search_value == current_item->first) {
			return current_item;
		}
		else if (map.key_comp()(search_value, current_item->first)) {
			if (index == search_end) {
				return (++current_item);
			}
			search_start = index + 1;
		}
		else {
			if (index == search_start) {
				return current_item;
			}
			search_end = index - 1;
		}
	}
}

void search_display_reverse(std::map<Date, int> & map, Date const & search_value) {
	std::map<Date, int>::const_reverse_iterator res = binary_search_map_reverse(map, search_value);
	if (res != map.rend()) {
		if (res->first != search_value) {
			std::cout << "Closest match to " << search_value << ": ";
		}
		std::cout << res->first << " => " << res->second << std::endl;
	}
	else {
		std::cout << "Earlier than anything (" << search_value << ")" << std::endl;
	}
}

void search_display(std::map<Date, int> & map, Date const & search_value) {
	std::map<Date, int>::const_iterator res = binary_search_map(map, search_value);
	if (res != map.end()) {
		if (res->first != search_value) {
			std::cout << "Closest match to " << search_value << ": ";
		}
		std::cout << res->first << " => " << res->second << std::endl;
	}
	else {
		std::cout << "Later than anything (" << search_value << ")" << std::endl;
	}
}

int main(int argc, char **argv) {
	std::atexit(&check_leaks);
	std::cout << std::boolalpha;

	/**//*/
	{
		std::map<Date, int> map;

		map[Date(2050, 1, 1)] = 3;
		map[Date(2040, 1, 1)] = 79;
		map[Date(2035, 1, 1)] = 1;
		map[Date(2045, 1, 1)] = 6;
		search_display_reverse(map, Date(2035, 1, 1));
		search_display_reverse(map, Date(2040, 1, 1));
		search_display_reverse(map, Date(2045, 1, 1));
		search_display_reverse(map, Date(2050, 1, 1));
		search_display_reverse(map, Date(2037, 1, 1));
		search_display_reverse(map, Date(2020, 1, 1));
		search_display_reverse(map, Date(2090, 1, 1));
		std::cout << std::endl << std::endl;
		search_display(map, Date(2035, 1, 1));
		search_display(map, Date(2040, 1, 1));
		search_display(map, Date(2045, 1, 1));
		search_display(map, Date(2050, 1, 1));
		search_display(map, Date(2037, 1, 1));
		search_display(map, Date(2020, 1, 1));
		search_display(map, Date(2090, 1, 1));
		exit(0);
	}
	/*/

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
		std::cout << e.what() << std::endl;
		throw ;
	}

	return (EXIT_SUCCESS);
}
