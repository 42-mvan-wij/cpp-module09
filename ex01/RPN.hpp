#pragma once
#ifndef RPN_HPP
# define RPN_HPP

# include <string>
# include <deque>

class RPN {
	public:
		RPN(std::string rpn);
		int calculate() const;

		class NoCalculation : public std::exception {
			public:
				char const * what() const throw();
		};

		class NotAnInteger : public std::exception {
			public:
				char const * what() const throw();
		};

		class NotEnoughOperands : public std::exception {
			public:
				char const * what() const throw();
		};

		class NotEnoughOperators : public std::exception {
			public:
				char const * what() const throw();
		};

		class StreamFail : public std::exception {
			public:
				char const * what() const throw();
		};

		class Overflow : public std::exception {
			public:
				char const * what() const throw();
		};

		class DivideByZero : public std::exception {
			public:
				char const * what() const throw();
		};

		RPN(RPN const & src);
		virtual ~RPN();
		RPN & operator=(RPN const & src);


	private:
		std::deque<std::string> _tokens;
		RPN();

		static int _checked_add(int a, int b);
		static int _checked_subtract(int a, int b);
		static int _checked_multiply(int a, int b);
		static int _checked_divide(int a, int b);

};

#endif
