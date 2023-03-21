#pragma once
#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <string>
# include <map>
# include <istream>
# include "Date.hpp"

class BitcoinExchange {
	public:
		BitcoinExchange(std::string exchange_filename);

		void parse(std::istream & istream) const;

		BitcoinExchange(BitcoinExchange const & src);
		virtual ~BitcoinExchange();
		BitcoinExchange & operator=(BitcoinExchange const & src);

		class ParseError : public std::exception {
			public:
				char const * what() const throw();
		};

		class DuplicateDate : public std::exception {
			public:
				char const * what() const throw();
		};

		class EmptyFinalLine : public ParseError {
			public:
				char const * what() const throw();
		};

		class InvalidFormat : public ParseError {
			public:
				char const * what() const throw();
		};

		class InvalidValue : public ParseError {
			public:
				char const * what() const throw();
		};

		class StreamError : public std::exception {
			public:
				char const * what() const throw();
		};

		class ValueTooHigh : public InvalidValue {
			public:
				char const * what() const throw();
		};

		class ValueTooLow : public InvalidValue {
			public:
				char const * what() const throw();
		};

	private:
		typedef std::map<Date, double> map;

		static std::pair<Date, double> _parse_line(std::istream & istream, std::string sep);
		static map::const_reverse_iterator _binary_search_map_reverse(map const & map, map::key_type const & search_value);
		BitcoinExchange();

		map _exchange_rate;

};

#endif
