#include <fstream>
#include <sstream>
#include <iostream>
#include "BitcoinExchange.hpp"

// --- Public ---

BitcoinExchange::BitcoinExchange(std::string exchange_filename) {
	std::ifstream exchange_file(exchange_filename.c_str());
	if (exchange_file.fail() || !exchange_file.is_open()) {
		throw BitcoinExchange::StreamError();
	}

	std::string line;
	std::getline(exchange_file, line);
	if (line != "date,exchange_rate") {
		throw BitcoinExchange::InvalidFormat();
	}

	while (!exchange_file.fail() && !exchange_file.eof()) {
		try {
			std::pair<Date, double> pair(BitcoinExchange::_parse_line(exchange_file, ","));

			if (this->_exchange_rate.count(pair.first) != 0) {
				throw BitcoinExchange::DuplicateDate();
			}

			this->_exchange_rate.insert(pair);
		} catch (BitcoinExchange::EmptyFinalLine &) {
		}
	}
}

void BitcoinExchange::parse(std::istream & istream) const {
	std::string line;
	std::getline(istream, line);
	if (istream.fail()) {
		throw BitcoinExchange::StreamError();
	}
	if (line != "date | value") {
		throw BitcoinExchange::InvalidFormat();
	}
	while (!istream.fail() && !istream.eof()) {
		try {
			std::pair<Date, double> pair(BitcoinExchange::_parse_line(istream, " | "));
			if (pair.second < 0) {
				throw BitcoinExchange::ValueTooLow();
			}
			if (pair.second > 1000) {
				throw BitcoinExchange::ValueTooHigh();
			}

			map::const_reverse_iterator it = BitcoinExchange::_binary_search_map_reverse(this->_exchange_rate, pair.first);
			if (it == this->_exchange_rate.rbegin()) {
				// TODO: maybe just use a value of 0
				std::cout << "Date before recording" << std::endl;
				throw "Date before recording"; // TODO: make better error
			}
			std::cout << pair.first << " => " << pair.second << " = " << (pair.second * it->second) << std::endl;
		} catch (BitcoinExchange::EmptyFinalLine & e) {
		} catch (BitcoinExchange::ParseError & e) {
			std::cout << "Error: " << e.what() << std::endl;
		} catch (Date::InvalidDate & e) {
			std::cout << "Error: " << e.what() << std::endl;
		}
	}
}

char const * BitcoinExchange::ParseError::what() const throw() {
	return "Error occurred during parsing";
}

char const * BitcoinExchange::DuplicateDate::what() const throw() {
	return "Duplicate date";
}

char const * BitcoinExchange::EmptyFinalLine::what() const throw() {
	return "Empty final line in file";
}

char const * BitcoinExchange::InvalidFormat::what() const throw() {
	return "Invalid format";
}

char const * BitcoinExchange::InvalidValue::what() const throw() {
	return "Value out of range";
}

char const * BitcoinExchange::StreamError::what() const throw() {
	return "Stream operation error";
}

char const * BitcoinExchange::ValueTooHigh::what() const throw() {
	return "Value too high";
}

char const * BitcoinExchange::ValueTooLow::what() const throw() {
	return "Value too low";
}

// --- OCF ---

BitcoinExchange::BitcoinExchange(BitcoinExchange const &src) {
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
		throw BitcoinExchange::StreamError();
	}
	std::string line;
	std::getline(istream, line);
	if (line.empty() && istream.eof()) {
		throw BitcoinExchange::EmptyFinalLine();
	}
	if (istream.fail()) {
		throw BitcoinExchange::StreamError();
	}
	size_t pos = line.find(sep);
	if (pos == line.npos) {
		throw BitcoinExchange::InvalidFormat();
	}
	double d;
	std::stringstream ss(line.substr(pos + sep.length()));
	if (ss.eof()) {
		throw BitcoinExchange::InvalidFormat();
	}
	ss >> d;
	if (ss.fail() || !ss.eof()) {
		throw BitcoinExchange::InvalidFormat();
	}
	std::pair<Date, double> pair(line.substr(0, pos), d);
	return pair;
}

// static
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
