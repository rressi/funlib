#pragma once

#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <iostream>

namespace fun {

class ResultEmptyException: public std::runtime_error {

public:
	ResultEmptyException()
      : std::runtime_error("Empty Result cannot be unwrapped") {}
};	

template<class T>
class Result {

public:

	typedef std::allocator<T> allocator;
	typedef typename allocator::value_type value_type; 
    typedef typename allocator::reference reference;
    typedef typename allocator::const_reference const_reference;
    typedef typename allocator::difference_type difference_type;
    typedef typename allocator::size_type size_type;
    typedef const value_type * const_iterator;
    typedef const value_type * const_reverse_iterator; 

private:

	const value_type * _value;

public:

	// --- constructors --------------------------------------------------------

	Result() noexcept
		: _value()
	{ }

	template<class U = T>
	Result(const Result<U> & other) noexcept
		: _value(other._value)
	{ }

	template<class U = T>
	Result(Result<U> && other) noexcept
		: _value(other._value)
	{ }

	template<class U = T>
	Result(const U& value) noexcept
		: _value(&value)
	{ }

	Result(T&& value) = delete;

	// --- containers ----------------------------------------------------------

	bool empty() const noexcept {
		return _value == nullptr;
	}

	size_type size() const noexcept {
		return empty()? size_type(0): size_type(1);
	}

	const_iterator begin() const noexcept {
		return _value;
	}

	const_iterator cbegin() const noexcept {
		return _value;
	}

	const_reverse_iterator rbegin() const noexcept {
		return _value;
	}

	const_iterator end() const noexcept {
		return begin() + size();
	}

	const_iterator cend() const noexcept {
		return cbegin() + size();
	}

	const_reverse_iterator rend() const noexcept {
		return rbegin() + size();
	}

	// --- optional ------------------------------------------------------------

	operator bool() const noexcept {
		return _value != nullptr;
	}

	bool has_value() const noexcept {
		return _value != nullptr;
	}

	const_reference& value() const {
		if (_value == nullptr) {
			throw ResultEmptyException();
		}
		return *_value;
	}

	template<class U>
	value_type value_or(U && other_value) const noexcept {
		if (!has_value()) {
			return other_value;
		}
		return value();
	}

	void swap(Result<T> &other) noexcept {
		std::swap(_value, other._value);
	}

	void reset() noexcept {
		_value = 0;
	}

	void clear() noexcept {
		_value = 0;
	}

	const_reference& operator*() const {
		if (_value == nullptr) {
			throw ResultEmptyException();
		}
		return *_value;
	}

	const value_type* operator->() const {
		if (_value == nullptr) {
			throw ResultEmptyException();
		}
		return _value;
	}

	// --- comparisons --------------------------------------------------------

	template<class U = T>
	int compare(const Result<U> & other) const noexcept {

		if (this->_value == other._value) {
			return 0;
		} else if (_value == nullptr) {
			return -1;
		} else if (other._value == nullptr) {
			return 1;
		} 

		auto& a = this->value();
		auto& b = other.value();
		if (a < b) {
			return -1;
		} else if (a == b) {
			return 0;
		}
		return 1;
	}	

	template<class U = T>
	bool operator<(const Result<U> & other) const noexcept {
		return compare(other) < 0;
	}

	template<class U = T>
	bool operator<=(const Result<U> & other) const noexcept {
		return compare(other) <= 0;
	}

	template<class U = T>
	bool operator==(const Result<U> & other) const noexcept {
		return compare(other) == 0;
	}

	template<class U = T>
	bool operator!=(const Result<U> & other) const noexcept {
		return compare(other) != 0;
	}

	template<class U = T>
	bool operator>=(const Result<U> & other) const noexcept {
		return compare(other) >= 0;
	}

	template<class U = T>
	bool operator>(const Result<U> & other) const noexcept {
		return compare(other) > 0;
	}

	// --- assignment ----------------------------------------------------------

	template<class U = T>
	Result& operator=(const Result<U> & other) noexcept {
		_value = other._value;
		return *this;
	}

	template<class U = T>
	Result& operator=(Result<U> && other) noexcept {
		_value = other._value;
		return *this;
	}

	template<class U = T>
	Result& operator=(const U& value) noexcept {
		_value = &value;
		return *this;
	}

	Result& operator=(T&& value) = delete;


};

} // namespace fun

// --- I/O ---------------------------------------------------------------------

template< class T >
std::ostream& operator<<(std::ostream& stream,
	                     const fun::Result<T>& Result)
{
	if (Result) {
		stream << Result.value();
	} else {
		stream << "N/A";
	}

	return stream;
}

