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
		throw Date::InvalidDate();
	}
	char c;
	ss >> c;
	if (ss.fail() || c != '-') {
		throw Date::InvalidDate();
	}
	ss >> this->_month;
	if (ss.fail()) {
		throw Date::InvalidDate();
	}
	ss >> c;
	if (ss.fail() || c != '-') {
		throw Date::InvalidDate();
	}
	ss >> this->_day;
	if (ss.fail()) {
		throw Date::InvalidDate();
	}
	if (!ss.eof()) {
		std::cout << "error" << std::endl;
		throw Date::InvalidDate();
	}
	if (!this->_vali_date()) {
		throw Date::InvalidDate();
	}
}

size_t Date::get_year() const {
	return this->_year;
}

size_t Date::get_month() const {
	return this->_month;
}

size_t Date::get_day() const {
	return this->_day;
}

std::ostream & operator<<(std::ostream & ostream, Date const & date) {
	ostream << date.get_year() << "-" << date.get_month() << "-" << date.get_day();
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

char const * Date::InvalidDate::what() const throw() {
	return "Invalid Date";
}

// --- OCF ---

Date::Date(Date const & src) {
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
