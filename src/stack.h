#ifndef __STACK_H__
#define __STACK_H__

#include <cassert>    // assert

#include "cpu.h"
#include "errors.h" // Asm::Errors::OutOfRangeException

template <unsigned size>
class Stack {
  using OutOfRangeException = Asm::Errors::OutOfRangeException;
public:
  Stack() noexcept {
    static_assert(size > 0, "Stack size must be superior than 0");
  }
  void push(u8 value) {
    if (registers::S >= size) throw OutOfRangeException{"Stack overflow"};
    buffer_[registers::S] = value;
    registers::S++;
  }
  u8 pop() {
    if (registers::S == 0) throw OutOfRangeException{"Stack is empty"};
    registers::S--;
    return buffer_[registers::S];
  }

private:
  std::array<u8, size> buffer_;
};

#endif // __STACK_H__
