#include "interpreter.h"
#include "cpu.h"    // registers
#include "syntax.h" // is_inst

#include <bitset>     // std::bitset
#include <cassert>    // assert
#include <cctype>     // std::toupper

#include <stdexcept>  // std::runtime_error

//! @brief anonymous namespace
namespace {
  /**
   * @brief Turns a string into a upper string
   * @param str The string we work with
   * @returns str as upper string
   * @throw std::bad_alloc if std::string::op+= fails
   */
	std::string to_upper(std::string const& str) {
		std::string cpy;
		for (auto const c : str) {
			cpy += std::toupper(c);
		}
		return cpy;
	}
}

void Asm::Interpreter::intepret_instruction(std::string const& inst) {
	using namespace Asm::Syntax;
	auto instruction = inst.substr(0, inst.find(";"));
	if (!is_inst(instruction)) {
		throw std::runtime_error{"Invalid instruction '" + instruction + "'"};
	}
	auto op = extract_op(instruction);
	auto param1 = (has_1_parameter(instruction)) ? extract_param1(instruction) : "";
	auto param2 = (has_2_parameters(instruction)) ? extract_param2(instruction) : "";
	exec(to_upper(op), to_upper(param1), to_upper(param2));
}
