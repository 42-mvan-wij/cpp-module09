#include <fstream>
#include <sstream>
#include <iostream>
#include "BitcoinExchange.hpp"

#define YELLOW "\x1b[33m"
#define RESET_COLOR "\x1b[0m"

// --- Public --- //

BitcoinExchange::BitcoinExchange(std::string exchange_filename) {
	std::ifstream exchange_file(exchange_filename.c_str());
	if (exchange_file.fail() || !exchange_file.is_open()) {
		throw BitcoinExchange::StreamError();
	}

	std::string line;
	std::getline(exchange_file, line);
	if (line != "date,exchange_rate") {
		throw BitcoinExchange::InvalidFormat(line);
	}

	while (!exchange_file.fail() && !exchange_file.eof()) {
		try {
			std::pair<Date, double> pair(BitcoinExchange::_parse_line(exchange_file, ","));

			if (this->_exchange_rate.count(pair.first) != 0) {
				throw BitcoinExchange::DuplicateDate(pair.first);
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
		throw BitcoinExchange::InvalidFormat(line);
	}
	while (!istream.fail() && !istream.eof()) {
		try {
			std::pair<Date, double> pair(BitcoinExchange::_parse_line(istream, " | "));
			if (pair.second < 0) {
				throw BitcoinExchange::ValueTooLow(pair.second, 0);
			}
			if (pair.second > 1000) {
				throw BitcoinExchange::ValueTooHigh(pair.second, 1000);
			}

			map::const_reverse_iterator it = BitcoinExchange::_binary_search_map_reverse(this->_exchange_rate, pair.first);
			if (it == this->_exchange_rate.rend()) {
				throw BitcoinExchange::DateTooEarly(pair.first);
			}
			std::cout << pair.first << " => " << pair.second << " = " << (pair.second * it->second) << std::endl;
		} catch (BitcoinExchange::EmptyFinalLine & e) {
		} catch (BitcoinExchange::ParseError & e) {
			std::cout << YELLOW << "Error: " << RESET_COLOR << e.what() << std::endl;
		} catch (Date::InvalidDate & e) {
			std::cout << YELLOW << "Error: " << RESET_COLOR << e.what() << std::endl;
		}
	}
}

// --- Exception classes --- //

char const * BitcoinExchange::ParseError::what() const throw() {
	return "Error occurred during parsing";
}

BitcoinExchange::DateTooEarly::~DateTooEarly() throw() {}

BitcoinExchange::DateTooEarly::DateTooEarly(Date date) : ParseError() {
	std::stringstream ss;
	ss << "Date too early: " << date;
	this->_error_text = ss.str();
}

char const * BitcoinExchange::DateTooEarly::what() const throw() {
	return this->_error_text.c_str();
}

BitcoinExchange::DuplicateDate::~DuplicateDate() throw() {}

BitcoinExchange::DuplicateDate::DuplicateDate(Date date) {
	std::stringstream ss;
	ss << "Duplicate Date: " << date;
	this->_error_text = ss.str();
}

char const * BitcoinExchange::DuplicateDate::what() const throw() {
	return this->_error_text.c_str();
}

char const * BitcoinExchange::EmptyFinalLine::what() const throw() {
	return "Empty final line in file";
}

BitcoinExchange::InvalidFormat::~InvalidFormat() throw() {}

BitcoinExchange::InvalidFormat::InvalidFormat() : ParseError() {
	std::stringstream ss;
	ss << "Invalid format";
	this->_error_text = ss.str();
}

BitcoinExchange::InvalidFormat::InvalidFormat(std::string str) : ParseError() {
	std::stringstream ss;
	ss << "Invalid format: \"" << str << "\"";
	this->_error_text = ss.str();
}

char const * BitcoinExchange::InvalidFormat::what() const throw() {
	return this->_error_text.c_str();
}

BitcoinExchange::InvalidValue::~InvalidValue() throw() {}

BitcoinExchange::InvalidValue::InvalidValue(double value) : ParseError() {
	std::stringstream ss;
	ss << "Value out of range: " << value;
	this->_error_text = ss.str();
}

char const * BitcoinExchange::InvalidValue::what() const throw() {
	return this->_error_text.c_str();
}

char const * BitcoinExchange::StreamError::what() const throw() {
	return "Stream operation error";
}


BitcoinExchange::ValueTooHigh::~ValueTooHigh() throw() {}

BitcoinExchange::ValueTooHigh::ValueTooHigh(double value) : InvalidValue(value) {
	std::stringstream ss;
	ss << "Value too high: " << value;
	this->_error_text = ss.str();
}

BitcoinExchange::ValueTooHigh::ValueTooHigh(double value, double highest) : InvalidValue(value) {
	std::stringstream ss;
	ss << "Value too high: " << value << ", cannot be higher than: " << highest;
	this->_error_text = ss.str();
}

BitcoinExchange::ValueTooLow::~ValueTooLow() throw() {}

BitcoinExchange::ValueTooLow::ValueTooLow(double value) : InvalidValue(value) {
	std::stringstream ss;
	ss << "Value too low: " << value;
	this->_error_text = ss.str();
}

BitcoinExchange::ValueTooLow::ValueTooLow(double value, double lowest) : InvalidValue(value) {
	std::stringstream ss;
	ss << "Value too low: " << value << ", cannot be lower than: " << lowest;
	this->_error_text = ss.str();
}

// --- OCF --- //

BitcoinExchange::BitcoinExchange(BitcoinExchange const &src) {
	*this = src;
}

BitcoinExchange::~BitcoinExchange() {
}

BitcoinExchange &BitcoinExchange::operator=(BitcoinExchange const &src) {
	this->_exchange_rate = src._exchange_rate;
	return *this;
}

// --- Protected --- //

// --- Private --- //

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
		throw BitcoinExchange::InvalidFormat(line);
	}
	double d;
	std::stringstream ss(line.substr(pos + sep.length()));
	if (ss.eof()) {
		throw BitcoinExchange::InvalidFormat(line);
	}
	ss >> d;
	if (ss.fail() || !ss.eof()) {
		throw BitcoinExchange::InvalidFormat(line);
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
