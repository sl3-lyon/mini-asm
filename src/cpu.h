#ifndef __CPU_H__
#define __CPU_H__

#include <array>      // std::array
#include <cstdint>    // uint8_t, uint16_t
#include <vector>     // std::vector

using u8 = uint8_t;
using u16 = uint16_t;

namespace registers {
  extern u8 A;  // Accumulator
  extern u8 X;  // Index X
  extern u8 Y;  // Index Y
  extern u8 P;  // Program status
  extern u16 PC; // Program Counter
  extern u8 S;  // Stack pointer
} // namespace registers

//!< @brief Different flags for CMP
enum Flags {
  negative = 0x01, //!< @brief "Negative" flag
  equal = 0x02,    //!< @brief "Equal" flag
  lower = 0x03,    //!< @brief "Lower" flag
  greater = 0x04   //!< @brief "Greater" flag
}; // namespace flags

namespace Asm {
template <unsigned size>
class Stack;
}
extern Asm::Stack<0xff> stack;
extern std::array<u8, 0xffff> RAM;
extern std::vector<u8> ROM;

#endif // __CPU_H__
