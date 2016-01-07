#ifndef __STRMANIP_H__
#define __STRMANIP_H__

#include <cctype>  // std::tolower, std::toupper
#include <string>  // std::string

/**
 * @brief Turns a string into a lower string
 * @param str The string we work with
 * @returns str as lower string
 * @throw std::bad_alloc if std::string::op+= fails
 */
inline std::string to_lower(std::string const& str) {
  std::string cpy;
  for (auto const c : str) {
    cpy += std::tolower(c);
  }
  return cpy;
}
/**
 * @brief Turns a string into a lower string
 * @param str The string we work with
 * @returns str as lower string
 * @throw std::bad_alloc if std::string::op+= fails
 */
inline std::string to_upper(std::string const& str) {
  std::string cpy;
  for (auto const c : str) {
    cpy += std::toupper(c);
  }
  return cpy;
}

#endif // __STRMANIP_H__
