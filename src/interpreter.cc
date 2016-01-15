#include "interpreter.h"
#include "cpu.h"      // registers
#include "strmanip.h" // to_lower, to_upper
#include "syntax.h"   // is_inst

#include <bitset>     // std::bitset
#include <cassert>    // assert
#include <regex>      // std::regex, std::regex_match
#include <sstream>    // std::stringstream
#include <stdexcept>  // std::runtime_error

void exec(std::string const& op, std::string const& param1, std::string const& param2);

/*
 * @brief Interprets an ASM instruction
 * @param inst Full ASM instruction (ex: "mov A, 42)
 * @throw std::runtime_error If inst is not a correct ASM instruction
 */
void Asm::Interpreter::intepret_instruction(std::string const& inst) {
  using namespace Asm::Syntax;
  auto instruction = to_lower(inst.substr(0, inst.find(";")));
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
  assert(is_lower(name) && "String must be a lower string");
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

inline bool is_lower(std::string const& str) noexcept {
  for (auto const& c : str) {
    if (c >= 'a' && c <= 'Z' && !islower(c)) {
      return false;
    }
  }
  return true;
}

/**
 * @brief Returns a reference to one of the registers
 * @param name Name of a register
 * @pre name must be a name of a register
 * @throw std::runtime_error if name is not correct
 */
u8& get_register(std::string const& name) {
  assert(is_lower(name) && "String must be a lower string");
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

u8 index_from(std::string const& val) {
  assert(is_lower(val) && "String must be a lower string");
  if (std::regex_match(val, std::regex{ "\\*[0-9]+" })) return static_cast<u8>(std::stoi(val.substr(1))); // Decimal
  if (std::regex_match(val, std::regex{"\\*0x[0-9a-f]+"})) return from_hex(val.substr(3)); // Hex
  if (std::regex_match(val, std::regex{"\\*0b[0-1]+"})) return from_bin(val.substr(3)); // Bin
  throw std::runtime_error{"Invalid token '" + val + "'"};
}

u8 value_of(std::string const& val) {
  assert(is_lower(val) && "String must be a lower string");
  // Register
  if (is_register(val)) return get_register(val);
  // Address
  if (is_address(val)) return RAM[index_from(val)];
  // Simple value
  else {
    if (std::regex_match(val, std::regex{"[0-9]+"})) return static_cast<u8>(std::stoi(val)); // Decimal
    if (std::regex_match(val, std::regex{"0x[0-9a-f]+"})) return from_hex(val.substr(2)); // Hex
    if (std::regex_match(val, std::regex{"0b[0-1]+"})) return from_bin(val.substr(2)); // Bin
  }
  throw std::runtime_error{"Invalid token '" + val + "'"};
}

inline u8& ref_to(std::string const& param) {
  assert(is_lower(param) && "String must be a lower string");
  if (is_register(param)) return get_register(param);
  if (is_address(param)) return RAM[index_from(param)];
  throw std::runtime_error{"Invalid value " + param};
}

void exec_mov(std::string const& param1, std::string const& param2) {
  assert(is_lower(param1) && "String must be a lower string");
  assert(is_lower(param2) && "String must be a lower string");
  if (is_register(param1)) ref_to(param1) = value_of(param2);
  else if (is_address(param1)) RAM[index_from(param1)] = value_of(param2);
  else throw std::runtime_error{"Cannot execute MOV\n"};
}

void exec_add(std::string const& param1, std::string const& param2) {
  assert(is_lower(param1) && "String must be a lower string");
  assert(is_lower(param2) && "String must be a lower string");
  ref_to(param1) += value_of(param2);
}

void exec_sub(std::string const& param1, std::string const& param2) {
  assert(is_lower(param1) && "String must be a lower string");
  assert(is_lower(param2) && "String must be a lower string");
  ref_to(param1) -= value_of(param2);
}

void exec_cmp(std::string const& param1, std::string const& param2) {
  assert(is_lower(param1) && "String must be a lower string");
  assert(is_lower(param2) && "String must be a lower string");
  auto val1 = value_of(param1);
  auto val2 = value_of(param2);
  registers::P = 0;
  if (val1 == val2) registers::P |= Flags::equal;
  else if (val1 > val2) registers::P |= Flags::greater;
  else if (val1 < val2) registers::P |= Flags::lower;
}

void exec_or(std::string const& param1, std::string const& param2) {
  assert(is_lower(param1) && "String must be a lower string");
  assert(is_lower(param2) && "String must be a lower string");
  ref_to(param1) |= value_of(param2);
}

void exec_and(std::string const& param1, std::string const& param2) {
  assert(is_lower(param1) && "String must be a lower string");
  assert(is_lower(param2) && "String must be a lower string");
  ref_to(param1) &= value_of(param2);
}

void exec_xor(std::string const& param1, std::string const& param2) {
  assert(is_lower(param1) && "String must be a lower string");
  assert(is_lower(param2) && "String must be a lower string");
  ref_to(param1) ^= value_of(param2);
}

void exec_push(std::string const& param1) {
  assert(is_lower(param1) && "String must be a lower string");
  stack.push(value_of(param1));
}

void exec_pop(std::string const& param1) {
  assert(is_lower(param1) && "String must be a lower string");
   ref_to(param1) = stack.pop();
}

inline void jump_if(std::string const& param1, bool cond) {
  assert(is_lower(param1) && "String must be a lower string");
  if (cond) {
    if (std::regex_match(param1, std::regex{"(0b[0-1]+|0x[0-9a-f]+|[0-9]+)"})) {
      u8 idx{};
      if (std::regex_match(param1, std::regex{"[0-9]+"})) idx = static_cast<u8>(std::stoi(param1)); // Decimal
      if (std::regex_match(param1, std::regex{"0x[0-9a-f]+"})) idx = from_hex(param1.substr(2)); // Hex
      if (std::regex_match(param1, std::regex{"0b[0-1]+"})) idx = from_bin(param1.substr(2)); // Bin
      registers::PC = idx;
    }
    else registers::PC = jmp_tokens[param1];
  }
}

void exec_jmp(std::string const& param1) {
  assert(is_lower(param1) && "String must be a lower string");
  jump_if(param1, true);
}

void exec_je(std::string const& param1) {
  assert(is_lower(param1) && "String must be a lower string");
  jump_if(param1, (registers::P & Flags::equal) != 0);
}

void exec_jne(std::string const& param1) {
  assert(is_lower(param1) && "String must be a lower string");
  jump_if(param1, !(registers::P & Flags::equal));
}

void exec_jl(std::string const& param1) {
  assert(is_lower(param1) && "String must be a lower string");
  jump_if(param1, (registers::P & Flags::lower) != 0);
}

void exec_jle(std::string const& param1) {
  assert(is_lower(param1) && "String must be a lower string");
  jump_if(param1, registers::P & Flags::lower || registers::P & Flags::equal);
}

void exec_jg(std::string const& param1) {
  assert(is_lower(param1) && "String must be a lower string");
  jump_if(param1, (registers::P & Flags::greater) != 0);
}

void exec_jge(std::string const& param1) {
  assert(is_lower(param1) && "String must be a lower string");
  jump_if(param1, registers::P & Flags::greater || registers::P & Flags::equal);
}

void exec_shl(std::string const& param1, std::string const& param2) {
  assert(is_lower(param1) && "String must be a lower string");
  assert(is_lower(param2) && "String must be a lower string");
  ref_to(param1) <<= value_of(param2);
}

void exec_shr(std::string const& param1, std::string const& param2) {
  assert(is_lower(param1) && "String must be a lower string");
  assert(is_lower(param2) && "String must be a lower string");
  ref_to(param1) >>= value_of(param2);
}

void exec(std::string const& op, std::string const& param1, std::string const& param2) {
  assert(is_lower(op) && "String must be a lower string");
  assert(is_lower(param1) && "String must be a lower string");
  assert(is_lower(param2) && "String must be a lower string");
  if (op == "mov") {
    exec_mov(param1, param2);
  } else if (op == "add") {
    exec_add(param1, param2);
  } else if (op == "sub") {
    exec_sub(param1, param2);
  } else if (op == "cmp") {
    exec_cmp(param1, param2);
  } else if (op == "or") {
    exec_or(param1, param2);
  } else if (op == "and") {
    exec_and(param1, param2);
  } else if (op == "xor") {
    exec_xor(param1, param2);
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
  } else if (op == "shl") {
    exec_shl(param1, param2);
  } else if (op == "shr") {
    exec_shr(param1, param2);
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
