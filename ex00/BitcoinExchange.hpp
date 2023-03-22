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

		class DateTooEarly : public ParseError {
			public:
        		virtual ~DateTooEarly() throw();
				DateTooEarly(Date date);
				char const * what() const throw();
			private:
				std::string _error_text;
		};

		class DuplicateDate : public std::exception {
			public:
        		virtual ~DuplicateDate() throw();
				DuplicateDate(Date date);
				char const * what() const throw();
			private:
				std::string _error_text;
		};

		class EmptyFinalLine : public ParseError {
			public:
				char const * what() const throw();
		};

		class InvalidFormat : public ParseError {
			public:
        		virtual ~InvalidFormat() throw();
				InvalidFormat();
				InvalidFormat(std::string str);
				char const * what() const throw();
			private:
				std::string _error_text;
		};

		class InvalidValue : public ParseError {
			public:
        		virtual ~InvalidValue() throw();
				InvalidValue(double value);
				char const * what() const throw();
			protected:
				std::string _error_text;
		};

		class StreamError : public std::exception {
			public:
				char const * what() const throw();
		};

		class ValueTooHigh : public InvalidValue {
			public:
				virtual ~ValueTooHigh() throw();
				ValueTooHigh(double value);
				ValueTooHigh(double value, double highest);
		};

		class ValueTooLow : public InvalidValue {
			public:
				virtual ~ValueTooLow() throw();
				ValueTooLow(double value);
				ValueTooLow(double value, double lowest);
		};

	private:
		typedef std::map<Date, double> map;

		static std::pair<Date, double> _parse_line(std::istream & istream, std::string sep);
		static map::const_reverse_iterator _binary_search_map_reverse(map const & map, map::key_type const & search_value);
		BitcoinExchange();

		map _exchange_rate;

};

#endif
