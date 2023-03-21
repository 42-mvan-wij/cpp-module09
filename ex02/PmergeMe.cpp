#include "PmergeMe.hpp"
#include <sstream>

// --- Public ---

PmergeMe PmergeMe::from(char **args) {
	PmergeMe result;
	element_type e;

	result._element_count = 0;

	for (int i = 0; args[i] != NULL; i++) {
		std::stringstream ss;
		ss.str(args[i]);
		ss >> e;

		if (ss.fail() || !ss.eof()) {
			throw "Some error"; // TODO: upgrade error
		}

		result._container1.push_back(e);
		result._container2.push_back(e);
		++result._element_count;
	}
	return result;
}

void PmergeMe::sort() {
	PmergeMe::_sort(this->_container1, 0, this->_element_count - 1);
	PmergeMe::_sort(this->_container2, 0, this->_element_count - 1);
}

// --- OCF ---

PmergeMe::PmergeMe(const PmergeMe & src) {
	*this = src;
}

PmergeMe::~PmergeMe() {
}

PmergeMe &PmergeMe::operator=(PmergeMe const & src) {
	this->_container1 = src._container1;
	this->_container2 = src._container2;
	return *this;
}

// --- Protected ---

// --- Private ---

PmergeMe::PmergeMe() {
}

PmergeMe::container1::const_iterator PmergeMe::_binary_search(container1::const_iterator range_begin, container1::const_iterator range_end, element_type const & search_value) {
	container1::const_iterator::difference_type len = std::distance(range_begin, range_end) + 1;
	container1::const_iterator::difference_type search_start = 0;
	container1::const_iterator::difference_type search_end = len - 1;
	while (true) {
		container1::const_iterator::difference_type index = (search_start + search_end) / 2;
		if (search_value == *(range_begin + index)) {
			return (range_begin + index);
		}
		if (search_value < *(range_begin + index)) {
			if (index == search_start) {
				return (range_begin + index);
			}
			search_end = index - 1;
		}
		if (search_value > *(range_begin + index)) {
			if (index == search_end) {
				return (range_begin + index + 1);
			}
			search_start = index + 1;
		}
	}
}

PmergeMe::container2::const_iterator PmergeMe::_binary_search(container2::const_iterator range_begin, container2::const_iterator range_end, element_type const & search_value) {
	container2::const_iterator::difference_type len = std::distance(range_begin, range_end) + 1;
	container2::const_iterator::difference_type search_start = 0;
	container2::const_iterator::difference_type search_end = len - 1;
	while (true) {
		container2::const_iterator::difference_type index = (search_start + search_end) / 2;
		container2::const_iterator current_item = range_begin;
		std::advance(current_item, index);
		if (search_value == *current_item) {
			return current_item;
		}
		if (search_value < *current_item) {
			if (index == search_start) {
				return current_item;
			}
			search_end = index - 1;
		}
		if (search_value > *current_item) {
			if (index == search_end) {
				return (++current_item);
			}
			search_start = index + 1;
		}
	}
}

void PmergeMe::_insertion_sort(container1 & container, size_t min_index, size_t max_index) {
	for (size_t current_index = min_index + 1; current_index <= max_index; current_index++) {
		element_type value = container[current_index];
		container1::const_iterator insert_location = PmergeMe::_binary_search(container.begin() + min_index, container.begin() + current_index - 1, value);
		container.erase(container.begin() + current_index);
		container.insert(insert_location, value);
	}
}

void PmergeMe::_insertion_sort(container2 & container, size_t min_index, size_t max_index) {
	for (size_t current_index = min_index + 1; current_index <= max_index; current_index++) {
		container2::const_iterator current_item = container.begin();
		std::advance(current_item, current_index);
		element_type value = *current_item;

		container2::const_iterator min_item = container.begin();
		std::advance(min_item, min_index);
		container2::const_iterator before_current_item = current_item;
		std::advance(before_current_item, -1);
		container.erase(current_item);
		container2::const_iterator insert_location = PmergeMe::_binary_search(min_item, before_current_item, value);
		container.insert(insert_location, value);
	}
}

void PmergeMe::_merge(container1 & container, size_t min_index, size_t mid_index, size_t max_index) {
	while (mid_index != max_index && min_index != mid_index + 1) {
		if (container[min_index] <= container[mid_index + 1]) {
			min_index++;
		}
		else {
			// copy value at mid_index + 1
			element_type value = container[mid_index + 1];
			// remove value at mid_index + 1
			container.erase(container.begin() + mid_index + 1);
			// insert copied value between min_index - 1 and min_index
			container.insert(container.begin() + min_index, value);
			mid_index++;
			min_index++;
		}
	}
}

void PmergeMe::_merge(container2 & container, size_t min_index, size_t mid_index, size_t max_index) {
	while (mid_index != max_index && min_index != mid_index + 1) {
		container2::const_iterator min_item = container.begin();
		std::advance(min_item, min_index);

		container2::const_iterator current_item = container.begin();
		std::advance(current_item, mid_index + 1);
		element_type current_value = *current_item;

		if (*min_item <= current_value) {
			min_index++;
		}
		else {
			// remove value at mid_index + 1
			container.erase(current_item);
			// insert copied value between min_index - 1 and min_index
			container2::const_iterator inserter = container.begin();
			std::advance(inserter, min_index);
			container.insert(inserter, current_value);
			mid_index++;
			min_index++;
		}
	}
}

void PmergeMe::_sort(container1 & container, size_t min_index, size_t max_index) {
	size_t length = max_index - min_index + 1;
	if (length > PmergeMe::INSERTION_LENGTH + 1) {
		size_t mid_index = (min_index + max_index) / 2;
		PmergeMe::_sort(container, min_index, mid_index);
		PmergeMe::_sort(container, mid_index + 1, max_index);
		PmergeMe::_merge(container, min_index, mid_index, max_index);
	}
	else {
		PmergeMe::_insertion_sort(container, min_index, max_index);
	}
}

void PmergeMe::_sort(container2 & container, size_t min_index, size_t max_index) {
	size_t length = max_index - min_index + 1;
	if (length > PmergeMe::INSERTION_LENGTH + 1) {
		size_t mid_index = (min_index + max_index) / 2;
		PmergeMe::_sort(container, min_index, mid_index);
		PmergeMe::_sort(container, mid_index + 1, max_index);
		PmergeMe::_merge(container, min_index, mid_index, max_index);
	}
	else {
		PmergeMe::_insertion_sort(container, min_index, max_index);
	}
}

std::ostream & operator<<(std::ostream & ostream, PmergeMe const & pmerge_me) {
	ostream << "container1: ";
	for (PmergeMe::container1::const_iterator it = pmerge_me._container1.begin(); it != pmerge_me._container1.end(); ++it) {
		ostream << *it << " ";
	}
	ostream << std::endl;
	ostream << "container2: ";
	for (PmergeMe::container2::const_iterator it = pmerge_me._container2.begin(); it != pmerge_me._container2.end(); ++it) {
		ostream << *it << " ";
	}
	return ostream;
}
