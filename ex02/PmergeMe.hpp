#pragma once
#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <vector>
# include <list>

class PmergeMe {
	public:
		PmergeMe(PmergeMe const & src);
		virtual ~PmergeMe();
		PmergeMe &operator=(PmergeMe const & src);

		static PmergeMe from(char **args);

		void sort();

		class NotANumber : public std::exception {
			public:
				char const * what() const throw();
		};

	private:
		typedef unsigned int element_type;
		typedef std::vector<element_type> container1;
		typedef std::list<element_type> container2;

		static size_t const INSERTION_LENGTH = 5;

		size_t _element_count;

		container1 _container1;
		container2 _container2;

		PmergeMe();

		static container1::const_iterator _binary_search(container1::const_iterator range_begin, container1::const_iterator range_end, element_type const & search_value);
		static container2::const_iterator _binary_search(container2::const_iterator range_begin, container2::const_iterator range_end, element_type const & search_value);

		// static void _insertion_sort(container1::const_iterator range_begin, container1::const_iterator range_end);
		// static void _insertion_sort(container2::const_iterator range_begin, container2::const_iterator range_end);

		static void _insertion_sort(container1 & container, size_t min_index, size_t max_index);
		static void _insertion_sort(container2 & container, size_t min_index, size_t max_index);

		// static void _merge(container1::const_iterator range_begin, container1::const_iterator range_end);
		// static void _merge(container2::const_iterator range_begin, container2::const_iterator range_end);

		static void _merge(container1 & container, size_t min_index, size_t mid_index, size_t max_index);
		static void _merge(container2 & container, size_t min_index, size_t mid_index, size_t max_index);

		static void _sort(container1 & container, size_t min_index, size_t max_index);
		static void _sort(container2 & container, size_t min_index, size_t max_index);
};

#endif
