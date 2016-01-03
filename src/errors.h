#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <exception> // std::exception
#include <string>    // std::string

namespace Asm {
namespace Errors {

class OutOfRangeException : public std::exception {
public:
	explicit OutOfRangeException(std::string const& what) : what_(what) {}
	const char* what() const noexcept override {
		return what_.c_str();
	}
private:
	std::string what_;
};

} // namespace Asm::Errors
} // namespace Asm

#endif // __ERRORS_H__
