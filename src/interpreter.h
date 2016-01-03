#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include "syntax.h"
#include <string> // std::string

namespace Asm {
namespace Interpreter {

void intepret_instruction(std::string const& inst, int line = -1);
void exec(std::string const& op, std::string const& param1, std::string const& param2);

} // namespace Asm
} // namespace Asm::Interpreter

#endif // __INTERPRETER_H__