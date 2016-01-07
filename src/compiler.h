#ifndef __COMPILER_H__
#define __COMPILER_H__

#include "cpu.h"
#include <functional> // std::function
#include <map>        // std::map

namespace Asm {
namespace Compiler {

extern std::map<u8, std::function<void(void)>> op; //!< @brief Mapping opcodes - operations

} // namespace Asm::Compiler
} // namespace Asm

#endif // __COMPILER_H__