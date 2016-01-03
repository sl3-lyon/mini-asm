#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "syntax.h"
#include <string> // std::string

namespace Asm {
namespace Interpreter {

void intepret_instruction(std::string const& inst);

} // namespace Asm
} // namespace Asm::Interpreter

#endif // __INTERPRETER_H__