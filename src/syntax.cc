#include "syntax.h"

#include <cassert> // assert
#include <cctype>  // std::isspace, std::tolower

//! @brief anonymous namespace
namespace {
  /**
   * @brief Turns a string into a lower string
   * @param str The string we work with
   * @returns str as lower string
   * @throw std::bad_alloc if std::string::op+= fails
   */
  std::string to_lower(std::string const& str) {
    std::string cpy;
    for (auto const c : str) {
      cpy += std::tolower(c);
    }
    return cpy;
  }
}

/**
 * @brief Checks if line is a correct ASM instruction
 * @param line The line we work with
 * @returns True if line is a correct ASM instruction, false otherwise
 * @throw /
 */
bool Asm::Syntax::is_inst(std::string const& line) noexcept {
  for (auto const& regex : Asm::Syntax::regexes) {
    if (std::regex_match(to_lower(line), regex)) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Checks if str matches any of the regexes
 * @param str The string we test
 * @param regexes A vector of regex
 * @pre Each regex must be correct
 * @returns true if str match any of the regexes, false otherwise
 * @throw std::regex_error If any regexe is not correct
 */
inline bool match_any(std::string const& str, std::vector<std::regex> regexes) {
  for (auto const& regex : regexes) {
    if (std::regex_match(to_lower(str), regex)) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Checks if the ASM instruction has at leat 1 parameter
 * @param line The line we work with
 * @returns True if the ASM instruction has at leat 1 parameter, false otherwise
 */
bool Asm::Syntax::has_1_parameter(std::string const& line) {
  return match_any(line, { regex_mov,
    regex_add, regex_sub, regex_cmp, regex_or,
    regex_and, regex_xor, regex_push, regex_pop,
    regex_jmp, regex_je, regex_jg, regex_jge,
    regex_jl, regex_jle, regex_jmp, regex_jne
  });
}

/**
 * @brief Checks if line is a correct ASM instruction
 * @param line The line we work with
 * @returns True if line is a correct ASM instruction, false otherwise
 */
bool Asm::Syntax::has_2_parameters(std::string const& line) {
  return match_any(line, { regex_mov,
    regex_add, regex_sub, regex_cmp,
    regex_or, regex_and, regex_xor
  });
}

/**
 * @brief Returns the ASM operand of the line
 * @pre line must be a correct ASM instruction
 * @param line The line we work with
 * @returns The operand
 * @throw std::bad_alloc if std::string::op+= fails
 */
std::string Asm::Syntax::extract_op(std::string const& line) {
  assert(is_inst(line));
  unsigned i{};
  std::string inst;
  for (; i < line.size() && std::isspace(line[i]); i++); // Skip all spaces
  for (; i < line.size() && !std::isspace(line[i]) && line[i] != ','; i++) {
    inst += line[i];
  }
  return inst;
}

/**
 * @brief Returns the ASM first param of the line
 * @pre line must be a correct ASM instruction
 * @pre The instruction must have at least 1 parameter
 * @param line The line we work with
 * @returns The parameter
 * @throw std::bad_alloc if std::string::op+= fails
 */
std::string Asm::Syntax::extract_param1(std::string const& line) {
  assert(has_1_parameter(to_lower(line)));
  unsigned i{};
  for (; i < line.size() && std::isspace(line[i]); i++); // Skip all spaces
  for (; i < line.size() && !std::isspace(line[i]) && line[i] != ','; i++); // Skip operand
  for (; i < line.size() && std::isspace(line[i]); i++); // Skip all spaces
  std::string param1;
  for (; i < line.size() && !std::isspace(line[i]) && line[i] != ',' &&  line[i] != ';'; i++) {
    param1 += line[i];
  }
  return param1;
}

/**
 * @brief Returns the ASM first param of the line
 * @pre line must be a correct ASM instruction
 * @pre The instruction must have 2 parameters
 * @param line The line we work with
 * @returns The parameter
 * @throw std::bad_alloc if std::string::op+= fails
 */
std::string Asm::Syntax::extract_param2(std::string const& line) {
  assert(has_2_parameters(to_lower(line)));
  unsigned i{};
  for (; i < line.size() && std::isspace(line[i]); i++); // Skip all spaces
  for (; i < line.size() && !std::isspace(line[i]) && line[i] != ','; i++); // Skip operand
  for (; i < line.size() && std::isspace(line[i]); i++); // Skip all spaces
  for (; i < line.size() && !std::isspace(line[i]); i++); // Skip param 1
  for (; i < line.size() && (std::isspace(line[i]) || line[i] == ','); i++); // Skip all spaces
  std::string param;
  for (; i < line.size() && !std::isspace(line[i]) && line[i] != ';'; i++) {
    param += line[i];
  }
  return param;
}
