#include <iostream>

#include "cpu.h"
#include "stack.h"

/**
 * @brief Swap the bytes of 2-bytes unsigned number
 * @returns A new u16 with the first's bytes swapped
 * @exception /
 */
constexpr u16 SwapBytes(u16 value) noexcept {
  return ((value & 0xff) << 8) + (value >> 8);
}

int main()
{
  /*Stack<0xff> stack;
  for (unsigned i{}; i < 0xff; i++) {
    stack.push(42);
  }
  for (unsigned i{}; i < 0xff; i++) {
    (void)stack.pop();
  }*/
  Stack<0xffff> stack;
  stack.push(42);
  stack.push(42);
  std::cout << int(registers::S);

  std::cin.get();
}
