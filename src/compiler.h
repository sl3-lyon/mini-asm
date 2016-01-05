#ifndef __COMPILER_H__
#define __COMPILER_H__

#include "cpu.h"
#include <functional> // std::function
#include <map>        // std::map

extern std::map<u8, std::function<void(void)>> op; //!< @brief Mapping opcode - operations

#endif // __COMPILER_H__
