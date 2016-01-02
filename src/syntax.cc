#include "syntax.h"

#include <cctype> // std::isspace
#include <assert.h>

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

	/**
   * @brief Checks if line is a correct ASM instruction
   * @param line The line we work with
   * @returns True if line is a correct ASM instruction, false otherwise
   * @throw /
   */
	bool is_inst(std::string const& line) noexcept {
		bool is_inst = false;
		for (auto const& regex : Asm::Syntax::regexes) {
			if (std::regex_match(to_lower(line), regex)) {
				is_inst = true;
				break;
			}
		}
		return is_inst;
	}

	/**
   * @brief Checks if line is a correct ASM instruction
   * @param line The line we work with
   * @returns True if line is a correct ASM instruction, false otherwise
   * @throw /
   */
	bool as_1_parameter(std::string const& line) noexcept {
		return false;
	}
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
	unsigned i{};
	for (; i < line.size() && std::isspace(line[i]); i++); // Skip all spaces
	for (; i < line.size() && !std::isspace(line[i]) && line[i] != ','; i++); // Skip operand
	for (; i < line.size() && std::isspace(line[i]); i++); // Skip all spaces
	std::string param1;
	for (; i < line.size() && !std::isspace(line[i]) && line[i] != ','; i++) {
		param1 += line[i];
	}
	return param1;
}