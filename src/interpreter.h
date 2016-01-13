#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "syntax.h"
#include <string> // std::string
#include "cpu.h"

namespace Asm {
namespace Interpreter {

void intepret_instruction(std::string const& inst);

} // namespace Asm
} // namespace Asm::Interpreter

u8& get_register(std::string const& name);
u8 value_of(std::string const& value);

#endif // __INTERPRETER_H__
