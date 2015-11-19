//#ifndef __STACK_H__
//#define __STACK_H__
//
//#include <array>     // std::array
//#include <cassert>   // assert
//#include <cstdint>   // uint8_t, uint16_t
//
//#include "errors.h"
//#include "cpu.h"
//
//template <unsigned size>
//class Stack {
//public:
//  Stack() = default;
//  void push(u8 value) {
//    assert(registers::S < size && "Stack overflow");
//    if (registers::S >= size) {
//      throw OutOfRangeException{ "Stack overflow" };
//    }
//    buffer_[registers::S] = value;
//    registers::S++;
//  }
//  u8 pop() {
//    assert(registers::S != 0 && "Stack is empty");
//    if (registers::S == 0) {
//      throw OutOfRangeException{ "Stack is empty" };
//    }
//    registers::S--;
//    return buffer_[registers::S];
//  }
//
//private:
//  std::array<u8, size> buffer_;
//};
//
//#endif // __STACK_H__
