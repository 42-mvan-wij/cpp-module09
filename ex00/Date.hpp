#pragma once
#ifndef DATE_HPP
# define DATE_HPP

class Date {
	public:
		Date(size_t year, size_t month, size_t day);
		Date(std::string date_string);

		bool operator==(Date const & rhs) const;
		bool operator<(Date const & rhs) const;

		bool operator!=(Date const & rhs) const;
		bool operator<=(Date const & rhs) const;
		bool operator>(Date const & rhs) const;
		bool operator>=(Date const & rhs) const;

		Date(Date const & src);
		virtual ~Date();
		Date &operator=(Date const & src);

		class InvalidDate : public std::exception {
			public:
				const char * what() const throw();
		};

	// private: // TODO: make private again
		size_t _year;
		size_t _month;
		size_t _day;

		Date();
		bool _vali_date() const;

};

std::ostream & operator<<(std::ostream & ostream, Date const & date);

#endif
