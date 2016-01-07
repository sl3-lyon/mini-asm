#ifndef __STACK_H__
#define __STACK_H__

#include <array>   // std::array
#include <cassert> // assert

#include "cpu.h"    // u8, registers
#include "errors.h" // Asm::Errors::OutOfRangeException

namespace Asm {

/**
 * @brief Stack of size "size"
 * @pre size must not be null
 */
template <unsigned size>
class Stack {
	using OutOfRangeException = Asm::Errors::OutOfRangeException;
public:
	/**
	 * @brief ctor
	 * @pre size must be superior than 0
	 * @throw /
	 */
	Stack() noexcept {
		static_assert(size > 0, "Stack size must be superior than 0");
	}

	/**
	 * @brief Push some value on the stack
	 * @param value The value to push
	 * @pre Stack must not be full
	 * @throw OutOfRangeException if the stack is full
	 */
	void push(u8 value) {
		if (registers::S >= size) throw OutOfRangeException{ "Stack overflow" };
		buffer_[registers::S] = value;
		registers::S++;
	}

	/**
	 * @brief Pop the value on top of the stack
	 * @pre Stack must not be empty
	 * @returns The value on top of the stack
	 * @throw OutOfRangeException if the stack is empty
	 */
	u8 pop() {
		if (registers::S == 0) throw OutOfRangeException{ "Stack is empty" };
		registers::S--;
		return buffer_[registers::S];
	}

private:
	std::array<u8, size> buffer_;
};

} // namespace Asm

#endif // __STACK_H__
