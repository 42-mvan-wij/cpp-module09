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

	private:
		typedef std::map<Date, double> map;

		std::map<std::string, std::string> _parse(std::istream & istream, std::string sep) const; // TODO: remove
		static std::pair<Date, double> _parse_line(std::istream & istream, std::string sep);

		static map::const_reverse_iterator _binary_search_map_reverse(map const & map, map::key_type const & search_value);
		BitcoinExchange();

		map _exchange_rate; // TODO: use better types

};

#endif
