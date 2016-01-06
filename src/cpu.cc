#include "cpu.h"
#include "stack.h" // Stack

Stack<0xff> stack{}; //!< @brief Stack

/**
 * @namespace registers
 * @brief Here are the registers used in MiniASM
 */
namespace registers {
  u8 A   = 0x00; //!< @brief Accumulator
  u8 X   = 0x00; //!< @brief X index
  u8 Y   = 0x00; //!< @brief Y index
  u8 P   = 0x00; //!< @brief Program status
  u16 PC = 0x00; //!< @brief Program counter
  u8 S   = 0x00; //!< @brief Stack pointer
} // namespace registers

std::vector<u8> ROM(1000);   //!< @brief Buffer in which will be loader the ROM
std::array<u8, 0xffff> RAM;  //!< @brief RAM
std::array<u8, 0xffff> VRAM; //!< @brief Video RAM (Not used yet)
