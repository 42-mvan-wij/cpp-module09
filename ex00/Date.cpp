#include <iostream>
#include <sstream>
#include "Date.hpp"

// --- Public ---

Date::Date(size_t year, size_t month, size_t day) : _year(year), _month(month), _day(day) {
	if (!this->_vali_date()) {
		throw Date::InvalidDate();
	}
}

Date::Date(std::string date_string) {
	std::stringstream ss(date_string);
	ss >> this->_year;
	if (ss.fail()) {
		std::cout << "error" << std::endl;
		throw "error"; // TODO: better error
	}
	char c;
	ss >> c;
	if (ss.fail() || c != '-') {
		std::cout << "error" << std::endl;
		throw "error"; // TODO: better error
	}
	ss >> this->_month;
	if (ss.fail()) {
		std::cout << "error" << std::endl;
		throw "error"; // TODO: better error
	}
	ss >> c;
	if (ss.fail() || c != '-') {
		std::cout << "error" << std::endl;
		throw "error"; // TODO: better error
	}
	ss >> this->_day;
	if (ss.fail()) {
		std::cout << "error" << std::endl;
		throw "error"; // TODO: better error
	}
	if (!ss.eof()) {
		std::cout << "error" << std::endl;
		throw "error"; // TODO: better error
	}
	if (!this->_vali_date()) {
		throw Date::InvalidDate();
	}
}

std::ostream & operator<<(std::ostream & ostream, Date const & date) {
	ostream << date._year << "-" << date._month << "-" << date._day;
	return ostream;
}

bool Date::operator==(Date const & rhs) const {
	return (this->_year == rhs._year && this->_month == rhs._month && this->_day == rhs._day);
}

bool Date::operator<(Date const & rhs) const {
	if (this->_year != rhs._year) {
		return (this->_year < rhs._year);
	}
	if (this->_month != rhs._month) {
		return (this->_month < rhs._month);
	}
	if (this->_day != rhs._day) {
		return (this->_day < rhs._day);
	}
	return (false);
}

bool Date::operator!=(Date const & rhs) const {
	return (!this->operator==(rhs));
}

bool Date::operator<=(Date const & rhs) const {
	return (this->operator==(rhs) || this->operator<(rhs));
}

bool Date::operator>(Date const & rhs) const {
	return (!this->operator<=(rhs));
}

bool Date::operator>=(Date const & rhs) const {
	return (!this->operator<(rhs));
}

const char * Date::InvalidDate::what() const throw() {
	return "Invalid Date";
}

// --- OCF ---

Date::Date(const Date & src) {
	*this = src;
}

Date::~Date() {
}

Date &Date::operator=(Date const & src) {
	this->_year = src._year;
	this->_month = src._month;
	this->_day = src._day;
	return *this;
}

// --- Protected ---

// --- Private ---

Date::Date() {
}

bool Date::_vali_date() const {
	if (this->_month < 1 || this->_month > 12) {
		return (false);
	}
	if (this->_day < 1) {
		return (false);
	}
	if (this->_month == 2 && this->_day > 28) {
		if (!(this->_day == 29 && this->_year % 4 == 0 && this->_year % 100 != 0)) {
			return (false);
		}
	}
	if (this->_month <= 7) {
		if (this->_month % 2 == 0) {
			if (this->_day > 30) {
				return (false);
			}
		}
		else {
			if (this->_day > 31) {
				return (false);
			}
		}
	}
	else {
		if (this->_month % 2 == 0) {
			if (this->_day > 31) {
				return (false);
			}
		}
		else {
			if (this->_day > 30) {
				return (false);
			}
		}
	}
	return (true);
}
