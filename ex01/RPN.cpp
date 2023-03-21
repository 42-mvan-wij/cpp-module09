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

// --- Public ---

RPN::RPN(std::string rpn) {
	this->_tokens = split(rpn, " ");

	size_t stack_size = 0;
	for (decltype(this->_tokens)::iterator it = this->_tokens.begin(); it != this->_tokens.end(); it++) {
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
			if (!ss.eof()) {
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

	for (decltype(this->_tokens)::const_iterator it = this->_tokens.begin(); it != this->_tokens.end(); it++) {
		std::string const & token = *it;
		if (token == "+" || token == "-" || token == "*" || token == "/") {
			int b = stack.back();
			stack.pop_back();
			int a = stack.back();
			stack.pop_back();
			switch (token.front()) {
				case '+':
					stack.push_back(a + b);
					break;
				case '-':
					stack.push_back(a - b);
					break;
				case '*':
					stack.push_back(a * b);
					break;
				case '/':
					stack.push_back(a / b);
					break;
			}
		}
		else {
			int i;
			std::stringstream ss(token);
			ss >> i;
			stack.push_back(i);
		}
	}

	return stack.front();
}

const char * RPN::NoCalculation::what() const throw() {
	return "No calculation to be done";
}

const char * RPN::NotAnInteger::what() const throw() {
	return "Not an integer";
}

const char * RPN::NotEnoughOperands::what() const throw() {
	return "Not enough operands";
}

const char * RPN::NotEnoughOperators::what() const throw() {
	return "Not enough operators";
}

// --- OCF ---

RPN::RPN(const RPN & src) {
	*this = src;
}

RPN::~RPN() {
}

RPN & RPN::operator=(RPN const & src) {
	this->_tokens = src._tokens;
	return *this;
}

// --- Protected ---

// --- Private ---

RPN::RPN() {
}