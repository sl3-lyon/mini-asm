#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <exception> // std::exception
#include <string>    // std::string

namespace Asm {
namespace Errors {

/*
 * @brief Exception thrown when we are out of a range
 */
class OutOfRangeException : public std::exception {
public:
  /**
   * @brief ctor
   * @param what The exception error text
   */
  explicit OutOfRangeException(std::string const& what) : what_(what) {}
  /**
   * @brief Getter to the exception error text
   * @returns The error text
   * @throw /
   */
  const char* what() const noexcept override {
    return what_.c_str();
  }
private:
  std::string what_;
};

} // namespace Asm::Errors
} // namespace Asm

#endif // __ERRORS_H__
