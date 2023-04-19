#include "RPN.hpp"
#include <sstream>

static std::deque<std::string> split(std::string str, std::string delim) {
	size_t found_pos;
	size_t pos = 0;
	std::deque<std::string> res;
	std::string s;

	found_pos = str.find(delim, pos);
	while (found_pos != str.npos) {
		s = str.substr(pos, found_pos - pos);
		if (!s.empty()) {
			res.push_back(s);
		}
		pos = found_pos + delim.length();
		found_pos = str.find(delim, pos);
	}
	s = str.substr(pos);
	if (!s.empty()) {
		res.push_back(s);
	}
	return res;
}

// --- Public --- //

RPN::RPN(std::string rpn) {
	this->_tokens = split(rpn, " ");

	size_t stack_size = 0;
	for (std::deque<std::string>::iterator it = this->_tokens.begin(); it != this->_tokens.end(); it++) {
		std::string & token = *it;
		if (token == "+" || token == "-" || token == "*" || token == "/") {
			if (stack_size < 2) {
				throw RPN::NotEnoughOperands();
			}
			stack_size -= 1;
		}
		else {
			stack_size += 1;
			int i;
			std::stringstream ss(token);
			ss >> i;
			if (ss.fail() || !ss.eof()) {
				throw RPN::NotAnInteger();
			}
		}
	}
	if (stack_size == 0) {
		throw RPN::NoCalculation();
	}
	else if (stack_size != 1) {
		throw RPN::NotEnoughOperators();
	}
}

int	RPN::calculate() const {
	std::deque<int> stack;

	for (std::deque<std::string>::const_iterator it = this->_tokens.begin(); it != this->_tokens.end(); it++) {
		std::string const & token = *it;
		if (token == "+" || token == "-" || token == "*" || token == "/") {
			int b = stack.back();
			stack.pop_back();
			int a = stack.back();
			stack.pop_back();
			switch (token[0]) {
				case '+':
					stack.push_back(RPN::_checked_add(a, b));
					break;
				case '-':
					stack.push_back(RPN::_checked_subtract(a, b));
					break;
				case '*':
					stack.push_back(RPN::_checked_multiply(a, b));
					break;
				case '/':
					stack.push_back(RPN::_checked_divide(a, b));
					break;
			}
		}
		else {
			int i;
			std::stringstream ss(token);
			ss >> i;
			if (ss.fail()) {
				throw RPN::StreamFail();
			}
			stack.push_back(i);
		}
	}

	return stack.front();
}

// --- Exception classes --- //

char const * RPN::NoCalculation::what() const throw() {
	return "No calculation to be done";
}

char const * RPN::NotAnInteger::what() const throw() {
	return "Not an integer";
}

char const * RPN::NotEnoughOperands::what() const throw() {
	return "Not enough operands";
}

char const * RPN::NotEnoughOperators::what() const throw() {
	return "Not enough operators";
}

char const * RPN::StreamFail::what() const throw() {
	return "Stream operation failed";
}

const char * RPN::Overflow::what() const throw() {
	return "Calculation resulted in over-/underflow";
}

const char * RPN::DivideByZero::what() const throw() {
	return "Calculation resulted in division by zero";
}

// --- OCF --- //

RPN::RPN(const RPN & src) {
	*this = src;
}

RPN::~RPN() {
}

RPN & RPN::operator=(RPN const & src) {
	this->_tokens = src._tokens;
	return *this;
}

// --- Protected --- //

// --- Private --- //

RPN::RPN() {
}

// static
int RPN::_checked_add(int a, int b) {
	bool a_is_positive = (a >= 0);
	bool b_is_positive = (b >= 0);
	if (a_is_positive != b_is_positive)
		return a + b;
	if (a > 0) {
		if (b > INT_MAX - a) {
			throw RPN::Overflow();
		}
		return a + b;
	}
	if (b < INT_MIN - a) {
		throw RPN::Overflow();
	}
	return a + b;
}

// static
int RPN::_checked_subtract(int a, int b) {
	if (b > 0) {
		return RPN::_checked_add(a, -b);
	}
	if (a > 0) {
		return -RPN::_checked_add(-a, b);
	}
	return a - b;
}

// static
int RPN::_checked_multiply(int a, int b) {
	if (a == 0 || b == 0) {
		return 0;
	}
	if (a > INT_MAX / b) {
		throw RPN::Overflow();
	}
	if (a < INT_MIN / b) {
		throw RPN::Overflow();
	}
	return a * b;
}

// static
int RPN::_checked_divide(int a, int b) {
	if (b == 0) {
		throw RPN::DivideByZero();
	}
	if (b == -1 && a == INT_MIN) {
		throw RPN::Overflow();
	}
	return a / b;
}
