#ifndef __SYNTAX_H__
#define __SYNTAX_H__

#include <regex> // std::regex

namespace Asm {
namespace Syntax {

std::regex const regex_mov{"mov[ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_add{"add[ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_sub{"sub[ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_cmp{"cmp[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_or{"or[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_and{"and[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_xor{"xor[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_push{"push[ \t]+([a|x|y|p|s]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+|'[a-zA-Z0-9]')[ \t]*"};
std::regex const regex_pop{"pop([ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+))?[ \t]*"};
std::regex const regex_jmp{"(jmp|je|jne|jl|jle|jg|jge)[ \t]+([a|x|y]|[0-9]+|0x[0-9a-f]+|0b[0-1]+)[ \t]*"};

const std::vector<std::regex> regexes = {
	regex_mov,
	regex_add,
	regex_sub,
	regex_cmp,
	regex_or,
	regex_and,
	regex_xor,
	regex_push,
	regex_pop,
	regex_jmp
};

const std::vector<std::string> instructions = {
  "mov A, 0xff",
  "mov X, 0xff",
  "cmp A, 0xff",
  "jne Y",
  "jle Y",
  "jl Y",
  "jge A",
  "je 0b1101",
  "mov *0xff, a",
  "mov A, *0x01",
  "mov X, 0x00",
  "push 42",
  "pop"
};

std::string extract_op(std::string const& line);
std::string extract_param1(std::string const& line);
std::string extract_param2(std::string const& line);

} // namespace Asm::Syntax
} // namespace Asm

#endif // __SYNTAX_H__
