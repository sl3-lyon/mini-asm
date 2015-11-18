#ifndef __CPU_H__
#define __CPU_H__

#include <cstdint>    // uint8_t, uint16_t
#include <functional> // std::function
#include <map>        // std::map
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

extern std::vector<u8> ROM;

extern const std::map<u8, std::function<void(void)>> op;

#endif // __CPU_H__
