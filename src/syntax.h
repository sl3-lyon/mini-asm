#ifndef __SYNTAX_H__
#define __SYNTAX_H__

#include <regex> // std::regex

const std::vector<std::regex> regexes = {
  std::regex{"mov[ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"},
  std::regex{"add[ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"},
  std::regex{"sub[ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"},
  std::regex{"cmp[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"},
  std::regex{"or[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"},
  std::regex{"and[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"},
  std::regex{"xor[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"},
  std::regex{"push[ \t]+([a|x|y|p|s]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+|'[a-zA-Z0-9]')[ \t]*"},
  std::regex{"pop([ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+))?[ \t]*"},
  std::regex{"(jmp|je|jne|jl|jle|jg|jge)[ \t]+([a|x|y]|[0-9]+|0x[0-9a-f]+|0b[0-1]+)[ \t]*"}
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

#endif // __SYNTAX_H__
