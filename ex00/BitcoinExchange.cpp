#include <fstream>
#include <sstream>
#include "BitcoinExchange.hpp"

// --- Public ---

#include <iostream> // TODO: remove
BitcoinExchange::BitcoinExchange(std::string exchange_filename) {
	std::ifstream exchange_file(exchange_filename);
	if (exchange_file.fail()) {
		// throw BitcoinExchange:: // TODO: throw some error
	}

	std::string line;
	std::getline(exchange_file, line);
	if (line != "date,exchange_rate") {
		std::cout << "Invalid format" << std::endl;
		throw "Invalid format"; // TODO: make proper error
	}


	while (!exchange_file.fail() && !exchange_file.eof()) {
		try {
			std::pair<Date, double> pair(BitcoinExchange::_parse_line(exchange_file, ","));

			if (this->_exchange_rate.count(pair.first) != 0) {
				std::cout << "Duplicate date" << std::endl;
				throw "Duplicate date";
			}

			this->_exchange_rate.insert(pair);
		} catch (char const * & e) {
			if (std::string(e) != "Empty final line") {
				throw ;
			}
		}
		// if (pair.second < 0) {
		// 	std::cout << "Negative value" << std::endl;
		// 	throw "Negative value";
		// }
		// if (pair.second > 1000) {
		// 	std::cout << "Value too high" << std::endl;
		// 	throw "Value too high";
		// }
	}




	// this->_exchange_rate = this->_parse(exchange_file, ",");
	// std::map<Date, float> mm;
	// for (decltype(this->_exchange_rate)::const_iterator it = this->_exchange_rate.begin(); it != this->_exchange_rate.end(); ++it) {
	// 	Date d(it->first);
	// 	if (mm.count(d) != 0) {
	// 		std::cout << "Duplicate date: " << d << " // " << it->first << std::endl;
	// 		throw "Duplicate date"; // TODO: make proper error
	// 	}
	// 	double f;
	// 	std::stringstream ss(it->second);
	// 	ss >> f;
	// 	if (ss.fail() || !ss.eof()) {
	// 		std::cout << "Not a floating point number" << std::endl;
	// 		throw "Not a floating point number"; // TODO: make proper error
	// 	}
	// 	mm[d] = f;
	// }
}

void BitcoinExchange::parse(std::istream & istream) const {
	std::string line;
	std::getline(istream, line);
	if (istream.fail()) {
		std::cout << "Fail" << std::endl;
		throw "Fail";
	}
	if (line != "date | value") {
		std::cout << "Invalid format" << std::endl;
		throw "Invalid format"; // TODO: make proper error
	}
	while (!istream.fail() && !istream.eof()) {
		try {
			std::pair<Date, double> pair(BitcoinExchange::_parse_line(istream, " | "));
			if (pair.second < 0) {
				std::cout << "Negative value" << std::endl;
				throw "Negative value";
			}
			if (pair.second > 1000) {
				std::cout << "Value too high" << std::endl;
				throw "Value too high";
			}

			map::const_reverse_iterator it = BitcoinExchange::_binary_search_map_reverse(this->_exchange_rate, pair.first);
			if (it == this->_exchange_rate.rbegin()) {
				// TODO: maybe just use a value of 0
				std::cout << "Date before recording" << std::endl;
				throw "Date before recording"; // TODO: make better error
			}
			std::cout << pair.first << " => " << pair.second << " = " << (pair.second * it->second) << std::endl;
		} catch (std::exception & e) { // TODO: maybe narrow down exception (and maybe use multiple catch statements)
			std::cerr << "\x1b[33m" << e.what() << "\x1b[0m" << std::endl;
		} catch (...) { // TODO: maybe narrow down exception (and maybe use multiple catch statements)
			std::cerr << "\x1b[33m" << "Unkown error" << "\x1b[0m" << std::endl;
		}
	}
	std::map<std::string, std::string> m = this->_parse(istream, " | ");
}

// --- OCF ---

BitcoinExchange::BitcoinExchange(const BitcoinExchange &src) {
	*this = src;
}

BitcoinExchange::~BitcoinExchange() {
}

BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange const &src) { // TODO: do something
	(void)src;
	return *this;
}

// --- Protected ---

// --- Private ---

BitcoinExchange::BitcoinExchange() {
}

// static
std::pair<Date, double> BitcoinExchange::_parse_line(std::istream & istream, std::string sep) {
	if (istream.eof()) {
		std::cout << "EOF" << std::endl;
		throw "EOF"; // TODO: make proper error
	}
	std::string line;
	std::getline(istream, line);
	if (line.empty() && istream.eof()) {
		std::cout << "Empty final line" << std::endl;
		throw "Empty final line";
	}
	if (istream.fail()) {
		std::cout << "Fail" << std::endl;
		throw "Fail";
	}
	size_t pos = line.find(sep);
	if (pos == line.npos) {
		std::cout << "Missing \"" << sep << "\": " << line << std::endl;
		throw "Missing separator"; // TODO: make proper error
	}
	double d;
	std::stringstream ss(line.substr(pos + sep.length()));
	if (ss.eof()) {
		std::cout << "Not a floating point number: " << line << std::endl;
		throw "Not a floating point number"; // TODO: make proper error
	}
	ss >> d;
	if (ss.fail() || !ss.eof()) {
		std::cout << "Not a floating point number: " << line << std::endl;
		throw "Not a floating point number"; // TODO: make proper error
	}
	std::pair<Date, double> pair(line.substr(0, pos), d);
	return pair;
}

std::map<std::string, std::string> BitcoinExchange::_parse(std::istream & istream, std::string sep) const {
	std::map<std::string, std::string> result;
	std::string line;

	while (!istream.eof()) {
		std::getline(istream, line);
		if (line == "" && istream.eof()) {
			break ;
		}
		size_t pos = line.find(sep);
		if (pos == line.npos) {
			std::cout << "Missing \"" << sep << "\"" << std::endl;
			throw "Missing separator"; // TODO: make proper error
		}
		std::string field1(line.substr(0, pos));
		std::string field2(line.substr(pos + sep.length()));
		result[field1] = field2;
		// std::cout << "Added " << field1 << " - \"" << sep << "\" - " << field2 << std::endl;
	}
	return result;
}

BitcoinExchange::map::const_reverse_iterator BitcoinExchange::_binary_search_map_reverse(map const & map, map::key_type const & search_value) {
	size_t len = map.size();
	size_t search_start = 0;
	size_t search_end = len - 1;
	while (true) {
		size_t index = (search_start + search_end) / 2;
		map::const_reverse_iterator current_item = map.rbegin();
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
