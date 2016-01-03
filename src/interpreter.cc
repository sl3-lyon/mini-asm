#include "interpreter.h"
#include "cpu.h"    // registers
#include "syntax.h" // is_inst

#include <bitset>     // std::bitset
#include <cassert>    // assert
#include <cctype>     // std::toupper
#include <functional> // std::function
#include <sstream>    // std::stringstream
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

inline bool is_register(std::string const& param) noexcept {
	auto op = to_upper(param);
	return op == "A" || op == "X" || op == "Y"
		|| op == "P" || op == "S";
}

inline bool is_address(std::string const& op) noexcept {
	return std::regex_match(op, std::regex{"\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+"});
}

inline u8& get_register(std::string const& reg) noexcept {
	auto upper_reg = to_upper(reg);
	if (upper_reg == "A") return registers::A;
	if (upper_reg == "X") return registers::X;
	if (upper_reg == "Y") return registers::Y;
	if (upper_reg == "P") return registers::P;
	if (upper_reg == "S") return registers::S;
	throw std::runtime_error{"Unknown register '" + upper_reg + "'"};
}

inline u8 from_hex(std::string const& hex) {
	unsigned x;
	std::stringstream ss;
	ss << std::hex << hex.substr(2);
	ss >> x;
	return static_cast<u8>(x);
}

inline u8 from_bin(std::string const& bin) {
	return static_cast<u8>(std::bitset<8>(bin.substr(2)).to_ulong());
}

inline u8 get_value_of(std::string const& val) {
	// Register
	if (is_register(val)) {
		return get_register(val);
	}
	// Address
	if (is_address(val)) {
		if (std::regex_match(val, std::regex{"\\*[0-9]+"})) return static_cast<u8>(std::stoi(val.substr(1))); // Decimal
		if (std::regex_match(val, std::regex{"\\*0x[0-9a-f]+"}))  return from_hex(val.substr(3)); // Hex
		if (std::regex_match(val, std::regex{"\\*0b[0-1]+"}))  return from_bin(val.substr(3)); // Bin
	}
	// Simple value
	else {
		if (std::regex_match(val, std::regex{"[0-9]+"})) return static_cast<u8>(std::stoi(val)); // Decimal
		if (std::regex_match(val, std::regex{"0x[0-9a-f]+"})) return from_hex(val.substr(2)); // Hex
		if (std::regex_match(val, std::regex{"0b[0-1]+"})) return from_bin(val.substr(2)); // Bin
	}
	throw std::runtime_error{"Unknown value '" + val + "'"};
}


// TODO - Refactoring
void exec_mov(std::string const& param1, std::string const& param2) {
	if (is_register(param1)) {
		get_register(param1) = get_value_of(param2);
	} else if (is_address(param1)) {
		RAM[get_value_of(param1)] = get_value_of(param2);
	} else {
		throw std::runtime_error{"Cannot execute MOV"};
	}
}

void exec_add(std::string const& param1, std::string const& param2) {
	if (is_register(param1)) {
		get_register(param1) += get_value_of(param2);
	} else if (is_address(param1)) {
		RAM[get_value_of(param1)] += get_value_of(param2);
	} else {
		throw std::runtime_error{"Cannot execute ADD"};
	}
}

void exec_sub(std::string const& param1, std::string const& param2) {
	if (is_register(param1)) {
		get_register(param1) -= get_value_of(param2);
	} else if (is_address(param1)) {
		RAM[get_value_of(param1)] -= get_value_of(param2);
	} else {
		throw std::runtime_error{"Cannot execute SUB"};
	}
}

void exec_cmp(std::string const& param1, std::string const& param2) {
	auto val1 = get_value_of(param1);
	auto val2 = get_value_of(param2);
	if (val1 == val2) {
		registers::P |= Flags::equal;
	} else if (val1 > val2) {
		registers::P |= Flags::greater;
	} else if (val1 < val2) {
		registers::P |= Flags::lower;
	}
}

void exec_or(std::string const& param1, std::string const& param2) {
	if (is_register(param1)) {
		get_register(param1) |= get_value_of(param2);
	} else if (is_address(param1)) {
		RAM[get_value_of(param1)] |= get_value_of(param2);
	} else {
		throw std::runtime_error{"Cannot execute OR"};
	}
}

void exec_and(std::string const& param1, std::string const& param2) {
	if (is_register(param1)) {
		get_register(param1) &= get_value_of(param2);
	} else if (is_address(param1)) {
		RAM[get_value_of(param1)] &= get_value_of(param2);
	} else {
		throw std::runtime_error{"Cannot execute AND"};
	}
}

void exec_xor(std::string const& param1, std::string const& param2) {
	if (is_register(param1)) {
		get_register(param1) ^= get_value_of(param2);
	} else if (is_address(param1)) {
		RAM[get_value_of(param1)] ^= get_value_of(param2);
	} else {
		throw std::runtime_error{"Cannot execute XOR"};
	}
}

void exec_push(std::string const& param1) {
	stack.push(get_value_of(param1));
}

void exec_pop(std::string const& param1) {
	if (is_register(param1)) {
		get_register(param1) = stack.pop();
	} else if (is_address(param1)) {
		RAM[get_value_of(param1)] = stack.pop();
	}
}
// End TODO

namespace Asm {
namespace Interpreter {

void exec(std::string const& op, std::string const& param1, std::string const& param2) {
	if (to_upper(op) == "MOV") {
		exec_mov(param1, param2);
	} else if (to_upper(op) == "ADD") {
		exec_add(param1, param2);
	} else if (to_upper(op) == "SUB") {
		exec_sub(param1, param2);
	} else if (to_upper(op) == "CMP") {
		exec_sub(param1, param2);
	} else if (to_upper(op) == "OR") {
		exec_sub(param1, param2);
	} else if (to_upper(op) == "AND") {
		exec_sub(param1, param2);
	} else if (to_upper(op) == "XOR") {
		exec_sub(param1, param2);
	} else if (to_upper(op) == "PUSH") {
		exec_push(param1);
	} else if (to_upper(op) == "POP") {
		exec_pop(param1);
	}
}

void Asm::Interpreter::intepret_instruction(std::string const& inst, int line) {
	using namespace Asm::Syntax;
	auto instruction = inst.substr(0, inst.find(";"));
	if (!is_inst(instruction)) {
		auto error = (line != -1)
			? "Invalid instruction line " + std::to_string(line)
			: "Invalid instruction";
		throw std::runtime_error{error};
	}
	auto op = extract_op(instruction);
	auto param1 = (has_1_parameter(instruction)) ? extract_param1(instruction) : "";
	auto param2 = (has_2_parameters(instruction)) ? extract_param2(instruction) : "";
	exec(op, param1, param2);
}

} // namespace Asm
} // namespace Asm::Interpreter