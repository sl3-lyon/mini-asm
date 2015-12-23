#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <exception> // std::exception
#include <string>    // std::string

class OutOfRangeException : public std::exception {
public:
	explicit OutOfRangeException(std::string const& what) : what_(what) {}
	const char* what() const noexcept override {
		return what_.c_str();
	}
private:
	std::string what_;
};

#endif // __ERRORS_H__
