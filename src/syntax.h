#ifndef __SYNTAX_H__
#define __SYNTAX_H__

#include <regex> // std::regex

namespace Asm {
namespace Syntax {

// TODO - Refactoring
std::regex const regex_mov{"[ \t]*mov[ \t]+([a|x|y|s|p|pc]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y|s|p|pc]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_add{"[ \t]*add[ \t]+([a|x|y|s|p|pc]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y|s|p|pc]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_sub{"[ \t]*sub[ \t]+([a|x|y|s|p|pc]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y|s|p|pc]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_cmp{"[ \t]*cmp[ \t]+[a|x|y|s|p|pc][ \t]*,[ \t]*([a|x|y|s|p|pc]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_or{"[ \t]*or[ \t]+[a|x|y|s|p|pc][ \t]*,[ \t]*([a|x|y|s|p|pc]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_and{"[ \t]*and[ \t]+[a|x|y|s|p|pc][ \t]*,[ \t]*([a|x|y|s|p|pc]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_xor{"[ \t]*xor[ \t]+[a|x|y|s|p|pc][ \t]*,[ \t]*([a|x|y|s|p|pc]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_push{"[ \t]*push[ \t]+([a|x|y|p|s]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+|'[a-zA-Z0-9]')[ \t]*"};
std::regex const regex_pop{"[ \t]*pop([ \t]+([a|x|y|s|p|pc]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+))?[ \t]*"};

std::regex const regex_jmp{"[ \t]*(jmp)[ \t]+[_a-zA-Z]*[_a-zA-Z0-9]+[ \t]*"};
std::regex const regex_je{"[ \t]*(je)[ \t]+[_a-zA-Z]*[_a-zA-Z0-9]+[ \t]*"};
std::regex const regex_jne{"[ \t]*(jne)[ \t]+[_a-zA-Z]*[_a-zA-Z0-9]+[ \t]*"};
std::regex const regex_jl{"[ \t]*(jl)[ \t]+[_a-zA-Z]*[_a-zA-Z0-9]+[ \t]*"};
std::regex const regex_jle{"[ \t]*(jle)[ \t]+[_a-zA-Z]*[_a-zA-Z0-9]+[ \t]*"};
std::regex const regex_jg{"[ \t]*(jg)[ \t]+[_a-zA-Z]*[_a-zA-Z0-9]+[ \t]*"};
std::regex const regex_jge{"[ \t]*(jge)[ \t]+[_a-zA-Z]*[_a-zA-Z0-9]+[ \t]*"};

std::regex const regex_shl{"[ \t]*shl[ \t]+([a|x|y|s|p|pc]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y|s|p|pc]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
std::regex const regex_shr{"[ \t]*shr[ \t]+([a|x|y|s|p|pc]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y|s|p|pc]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
// Fin TODO

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
	regex_jmp,
	regex_je,
	regex_jne,
	regex_jl,
	regex_jle,
	regex_jg,
	regex_jge,
	regex_shl,
	regex_shr
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

bool is_inst(std::string const& line) noexcept;
bool has_1_parameter(std::string const& line);
bool has_2_parameters(std::string const& line);
std::string extract_op(std::string const& line);
std::string extract_param1(std::string const& line);
std::string extract_param2(std::string const& line);

} // namespace Asm::Syntax
} // namespace Asm

#endif // __SYNTAX_H__
