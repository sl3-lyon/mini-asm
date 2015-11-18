#include "cpu.h"
#include "errors.h"

namespace registers {
  u8 A  = 0x00;
  u8 X  = 0x00;
  u8 Y  = 0x00;
  u8 P  = 0x00;
  u8 PC = 0x00;
  u8 S  = 0x00;
} // namespace registers

enum Flags {
  negative = 0x01,
  equal = 0x02,
  lower = 0x03,
  greater = 0x04
}; // namespace flags

std::vector<u8> ROM;
std::array<u8, 0xffff> RAM;
std::array<u8, 0xffff> VRAM;

u8 read_next() {
  if (registers::PC >= ROM.size()) {
    throw OutOfRangeException{"Out of ROM file"};
  }
  return ROM[registers::PC++];
}

u8 read_addr() {
  if (registers::PC >= ROM.size()) {
    throw OutOfRangeException{"Out of ROM file"};
  }
  return ROM[ROM[registers::PC++]];
}

inline void cmp(u8 ra, u8 rb) noexcept {
  if (ra == rb) {
    registers::P |= Flags::equal;
  } else if (ra < rb) {
    registers::P |= Flags::lower;
  } else if (ra > rb) {
    registers::P |= Flags::greater;
  }
}

inline void cmp() {
  if (registers::PC >= ROM.size()) {
    throw OutOfRangeException{"Out of ROM file"};
  }
  auto lop = ROM[registers::PC++];
  if (registers::PC >= ROM.size()) {
    throw OutOfRangeException{"Out of ROM file"};
  }
  auto rop = ROM[registers::PC++];
  cmp(lop, rop);
}

inline void cmpRegOp(u8 reg) {
  if (registers::PC >= ROM.size()) {
    throw OutOfRangeException{"Out of ROM file"};
  }
  auto op = ROM[registers::PC++];
  cmp(reg, op);
}

inline void cmpOpReg(u8 reg) {
  if (registers::PC >= ROM.size()) {
    throw OutOfRangeException{"Out of ROM file"};
  }
  auto op = ROM[registers::PC++];
  cmp(op, reg);
}

inline void jmp() {
  if (registers::PC >= ROM.size() || ROM[ROM[registers::PC]] >= ROM.size()) {
    throw OutOfRangeException{"Out of ROM file"};
  }
  registers::PC = ROM[ROM[registers::PC]];
}

inline void je() {
  if (registers::PC >= ROM.size()) {
    throw OutOfRangeException{ "Out of ROM file" };
  }
  auto op = ROM[registers::PC++];
  if (registers::P & Flags::equal) {
    registers::PC = op;
  }
}

inline void jne() {
  if (registers::PC >= ROM.size()) {
    throw OutOfRangeException{ "Out of ROM file" };
  }
  auto op = ROM[registers::PC++];
  if (!(registers::P & Flags::equal)) {
    registers::PC = op;
  }
}

inline void jl() {
  if (registers::PC >= ROM.size()) {
    throw OutOfRangeException{ "Out of ROM file" };
  }
  auto op = ROM[registers::PC++];
  if (registers::P & Flags::lower) {
    registers::PC = op;
  }
}

inline void jle() {
  if (registers::PC >= ROM.size()) {
    throw OutOfRangeException{ "Out of ROM file" };
  }
  auto op = ROM[registers::PC++];
  if (registers::P & Flags::lower || registers::P & Flags::equal) {
    registers::PC = op;
  }
}

inline void jg() {
  if (registers::PC >= ROM.size()) {
    throw OutOfRangeException{ "Out of ROM file" };
  }
  auto op = ROM[registers::PC++];
  if (registers::P & Flags::greater) {
    registers::PC = op;
  }
}

inline void jge() {
  if (registers::PC >= ROM.size()) {
    throw OutOfRangeException{ "Out of ROM file" };
  }
  auto op = ROM[registers::PC++];
  if (registers::P & Flags::greater || registers::P & Flags::equal) {
    registers::PC = op;
  }
}

inline void call() {
  if (registers::PC >= ROM.size()) {
    throw OutOfRangeException{ "Out of ROM file" };
  }
  auto op = ROM[registers::PC++];
  // TODO - Call
}

const std::map<u8, std::function<void(void)>> op = {
  { 0x00, []() -> void {} },

  // --[ MOV ]--
  { 0x01, []() -> void { registers::A = read_next(); } }, // mov A, VALUE
  { 0x02, []() -> void { registers::A = read_addr(); } }, // mov A, ADDR
  { 0x03, []() -> void { registers::X = read_next(); } }, // mov X, VALUE
  { 0x04, []() -> void { registers::X = read_addr(); } }, // mov X, ADDR
  { 0x05, []() -> void { registers::Y = read_next(); } }, // mov Y, VALUE
  { 0x06, []() -> void { registers::Y = read_addr(); } }, // mov Y, ADDR

  // --[ ADD ]--
  { 0x07, []() -> void { registers::A += read_next(); } }, // add A, VALUE
  { 0x08, []() -> void { registers::A += read_addr(); } }, // add A, ADDR
  { 0x09, []() -> void { registers::X += read_next(); } }, // add X, VALUE
  { 0x0a, []() -> void { registers::X += read_addr(); } }, // add X, ADDR
  { 0x0b, []() -> void { registers::Y += read_next(); } }, // add Y, VALUE
  { 0x0c, []() -> void { registers::Y += read_addr(); } }, // add Y, ADDR

  // --[ SUB ]--
  { 0x0d, []() -> void { registers::A -= read_next(); } }, // sub A, VALUE
  { 0x0e, []() -> void { registers::A -= read_addr(); } }, // sub A, ADDR
  { 0x0f, []() -> void { registers::X -= read_next(); } }, // sub X, VALUE
  { 0x10, []() -> void { registers::X -= read_addr(); } }, // sub X, ADDR
  { 0x11, []() -> void { registers::Y -= read_next(); } }, // sub Y, VALUE
  { 0x12, []() -> void { registers::Y -= read_addr(); } }, // sub Y, ADDR

  // --[ SHL ]--
  { 0x13, []() -> void { registers::A <<= read_next(); } }, // shl A, VALUE
  { 0x14, []() -> void { registers::X <<= read_next(); } }, // shl X, VALUE
  { 0x15, []() -> void { registers::Y <<= read_next(); } }, // shl Y, VALUE

  // --[ SHR ]--
  { 0x16, []() -> void { registers::A >>= read_next(); } }, // shl A, VALUE
  { 0x17, []() -> void { registers::X >>= read_next(); } }, // shl X, VALUE
  { 0x18, []() -> void { registers::Y >>= read_next(); } }, // shl Y, VALUE

  // --[ CMP ]--
  { 0x19, []() -> void { cmp(registers::A, registers::X); } }, // cmp A, X
  { 0x1A, []() -> void { cmp(registers::A, registers::Y); } }, // cmp A, Y
  { 0x1B, []() -> void { cmpRegOp(registers::A); } }, // cmp A, op
  { 0x1C, []() -> void { cmpOpReg(registers::A); } }, // cmp op, A

  { 0x1D, []() -> void { cmp(registers::X, registers::A); } }, // cmp X, A
  { 0x1E, []() -> void { cmp(registers::X, registers::Y); } }, // cmp X, Y
  { 0x1F, []() -> void { cmpRegOp(registers::X); } }, // cmp X, op
  { 0x20, []() -> void { cmpOpReg(registers::X); } }, // cmp op, X

  { 0x21, []() -> void { cmp(registers::Y, registers::A); } }, // cmp Y, A
  { 0x22, []() -> void { cmp(registers::Y, registers::X); } }, // cmp Y, X
  { 0x23, []() -> void { cmpRegOp(registers::Y); } }, // cmp Y, op
  { 0x24, []() -> void { cmpOpReg(registers::Y); } }, // cmp op, Y

  // --[ JMP ]--
  { 0x25, []() -> void { jmp(); } }, // jmp
  { 0x26, []() -> void { je(); } },  // je
  { 0x27, []() -> void { jne(); } }, // jne
  { 0x28, []() -> void { jl(); } },  // jl
  { 0x29, []() -> void { jle(); } }, // jle
  { 0x2A, []() -> void { jg(); } },  // jg
  { 0x2B, []() -> void { jge(); } }, // jge

  // --[ CALL ]--
  { 0x2C, []() -> void { call(); } }
};
