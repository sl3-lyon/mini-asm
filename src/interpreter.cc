#include "interpreter.h"
#include "cpu.h"    // registers
#include "syntax.h" // is_inst

#include <bitset>  // std::bitset
#include <cctype>  // std::toupper
#include <sstream> // std::stringstream
#include <regex>   // std::regex, std::regex_match

#include <stdexcept>  // std::runtime_error

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
   * @brief Turns a string into a lower string
   * @param str The string we work with
   * @returns str as lower string
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

void exec(std::string const& op, std::string const& param1, std::string const& param2);

void Asm::Interpreter::intepret_instruction(std::string const& inst) {
  using namespace Asm::Syntax;
  auto instruction = inst.substr(0, inst.find(";"));
  if (!is_inst(instruction)) {
    throw std::runtime_error{"Invalid instruction '" + instruction + "'\n"};
  }
  auto op = extract_op(instruction);
  auto param1 = (has_1_parameter(instruction)) ? extract_param1(instruction) : "";
  auto param2 = (has_2_parameters(instruction)) ? extract_param2(instruction) : "";
  exec(to_lower(op), to_lower(param1), to_lower(param2));
}

/**
 * @brief Checks if val is the name of a register
 * @param val The string we test
 * @returns True if val is the name of a register, false otherwise
 * @throw /
 */
inline bool is_register(std::string const& name) noexcept {
  return name == "a" || name == "x" || name == "y"
    || name == "p" || name == "pc" || name == "s";
}

/**
 * @brief Checks if val is an address
 * @param val The string we test
 * @returns True if val is an address, false otherwise
 * @throw /
 */
inline bool is_address(std::string const& val) noexcept {
  return std::regex_match(val, std::regex{"\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+"});
}

/**
 * @brief Returns a reference to one of the registers
 * @param name Name of a register
 * @pre name must be a name of a register
 * @throw std::runtime_error if name is not correct
 */
u8& get_register(std::string const& name) {
  if (name == "a") return registers::A;
  if (name == "x") return registers::X;
  if (name == "y") return registers::Y;
  if (name == "s" || name == "p" || name == "pc") {
    throw std::runtime_error{"Cannot access to register '" + to_upper(name) + "'\n"};
  }
  throw std::runtime_error{"Invalid token '" + name + "'\n"};
}

inline u8 from_hex(std::string const& hex) {
  unsigned x;
  std::stringstream ss;
  ss << std::hex << hex;
  ss >> x;
  return static_cast<u8>(x);
}

inline u8 from_bin(std::string const& bin) {
  return static_cast<u8>(std::bitset<8>(bin).to_ulong());
}

inline u8 get_value_of(std::string const& value) {
  auto val = to_lower(value);
  // Register
  if (is_register(val)) {
    return get_register(val);
  }
  // Address
  if (is_address(val)) {
    if (std::regex_match(val, std::regex{"\\*[0-9]+"})) return RAM[static_cast<u8>(std::stoi(val.substr(1)))]; // Decimal
    if (std::regex_match(val, std::regex{"\\*0x[0-9a-f]+"}))  return RAM[from_hex(val.substr(3))]; // Hex
    if (std::regex_match(val, std::regex{"\\*0b[0-1]+"}))  return RAM[from_bin(val.substr(3))]; // Bin
  }
  // Simple value
  else {
    if (std::regex_match(val, std::regex{"[0-9]+"})) return static_cast<u8>(std::stoi(val)); // Decimal
    if (std::regex_match(val, std::regex{"0x[0-9a-f]+"})) return from_hex(val.substr(2)); // Hex
    if (std::regex_match(val, std::regex{"0b[0-1]+"})) return from_bin(val.substr(2)); // Bin
  }
  throw std::runtime_error{"Invalid token '" + val + "'"};
}


// TODO - Refactoring
void exec_mov(std::string const& param1, std::string const& param2) {
  if (is_register(param1)) {
    get_register(param1) = get_value_of(param2);
  } else if (is_address(to_lower(param1))) {
    u8 idx = 0;
    if (std::regex_match(to_lower(param1), std::regex{"\\*[0-9]+"})) idx = static_cast<u8>(std::stoi(param1.substr(1))); // Decimal
    if (std::regex_match(to_lower(param1), std::regex{"\\*0x[0-9a-f]+"}))  idx = from_hex(param1.substr(3)); // Hex
    if (std::regex_match(to_lower(param1), std::regex{"\\*0b[0-1]+"}))  idx = from_bin(param1.substr(3)); // Bin
    RAM[idx] = get_value_of(param2);
  } else {
    throw std::runtime_error{"Cannot execute MOV\n"};
  }
}

u8& value_of(std::string const& param) {
  if (is_register(param)) return get_register(param);
  if (is_address(param))  return RAM[get_value_of(param)];
  throw std::runtime_error{"Invalid value " + param};
}

void exec_add(std::string const& param1, std::string const& param2) {
  value_of(param1) += get_value_of(param2);
}

void exec_sub(std::string const& param1, std::string const& param2) {
  value_of(param1) -= get_value_of(param2);
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
  value_of(param1) |= get_value_of(param2);
}

void exec_and(std::string const& param1, std::string const& param2) {
  value_of(param1) &= get_value_of(param2);
}

void exec_xor(std::string const& param1, std::string const& param2) {
  value_of(param1) ^= get_value_of(param2);
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

template <typename Fun>
inline void jump_if(std::string const& param1, Fun fun) {
  if (fun()) {
    registers::PC = get_value_of(param1);
  }
}

void exec_jmp(std::string const& param1) {
  jump_if(param1, []() -> bool { return true; });
}

void exec_je(std::string const& param1) {
  jump_if(param1, []() -> bool { return (registers::P & Flags::equal) != 0; });
}

void exec_jne(std::string const& param1) {
  jump_if(param1, []() -> bool { return !(registers::P & Flags::equal); });
}

void exec_jl(std::string const& param1) {
  jump_if(param1, []() -> bool { return (registers::P & Flags::lower) != 0; });
}

void exec_jle(std::string const& param1) {
  jump_if(param1, []() -> bool { return registers::P & Flags::lower || registers::P & Flags::equal; });
}

void exec_jg(std::string const& param1) {
  jump_if(param1, []() -> bool { return (registers::P & Flags::greater) != 0; });
}

void exec_jge(std::string const& param1) {
  jump_if(param1, []() -> bool { return registers::P & Flags::greater || registers::P & Flags::equal; });
}
// End TODO

void exec(std::string const& op, std::string const& param1, std::string const& param2) {
  if (op == "mov") {
    exec_mov(param1, param2);
  } else if (op == "add") {
    exec_add(param1, param2);
  } else if (op == "sub") {
    exec_sub(param1, param2);
  } else if (op == "cmp") {
    exec_sub(param1, param2);
  } else if (op == "or") {
    exec_sub(param1, param2);
  } else if (op == "and") {
    exec_sub(param1, param2);
  } else if (op == "xor") {
    exec_sub(param1, param2);
  } else if (op == "push") {
    exec_push(param1);
  } else if (op == "pop") {
    exec_pop(param1);
  } else if (op == "jmp") {
    exec_jmp(param1);
  } else if (op == "je") {
    exec_je(param1);
  } else if (op == "jne") {
    exec_jne(param1);
  } else if (op == "jl") {
    exec_jl(param1);
  } else if (op == "jle") {
    exec_jle(param1);
  } else if (op == "jg") {
    exec_jg(param1);
  } else if (op == "jge") {
    exec_jge(param1);
  }
}
