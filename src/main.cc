#include <fstream>  // std::ifstream
#include <iostream> // std::cout
#include <map>      // std::map

#include "cpu.h"
#include "infos.h"
#include "interpreter.h"
#include "strmanip.h" // to_lower, to_upper

/**
* @brief Swap the bytes of 2-bytes unsigned number
* @returns A new u16 with the first's bytes swapped
* @exception /
*/
constexpr u16 SwapBytes(u16 value) noexcept { // Not used yet
  return ((value & 0xff) << 8) + (value >> 8);
}

void start_shell_mode();
void read_from_file(std::string const& filename);

int main(int argc, char **argv) {
  try {
    if (argc == 1) {
      start_shell_mode();
    } else if (argc == 2) {
      read_from_file(std::string{argv[1]});
      start_shell_mode();
    } else {
      std::cout << "Wrong number of arguments: expected file name or no argument for shell mode";
    }
  } catch (std::exception const& e) {
    std::cout << std::string{"Error: "} + e.what();
  }
  std::cin.get();
}

const std::vector<std::regex> command_regexes = {
  std::regex{"print registers"},
  std::regex{"print (a|x|y|p|s|pc)"},
  std::regex{"print (\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)"}
};

bool is_command(std::string const& line) {
  for (auto const& regex : command_regexes) {
    if (std::regex_match(line, regex)) {
      return true;
    }
  }
  return false;
}

void print_registers() {
  std::cout << "Register A: " << static_cast<unsigned>(registers::A) << std::endl;
  std::cout << "Register X: " << static_cast<unsigned>(registers::X) << std::endl;
  std::cout << "Register Y: " << static_cast<unsigned>(registers::Y) << std::endl;
  std::cout << "Register P: " << static_cast<unsigned>(registers::P) << std::endl;
  std::cout << "Register PC: " << static_cast<unsigned>(registers::PC) << std::endl;
  std::cout << "Register S: " << static_cast<unsigned>(registers::S) << std::endl;
}

void interpret_command(std::string const& command) {
  if (command == "print registers") {
    print_registers();
  } else if (std::regex_match(command, std::regex{"print (a|x|y|p|s|pc)"})
    || std::regex_match(command, std::regex{"print (\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)"})) {
    std::cout << static_cast<unsigned>(value_of(command.substr(6))) << "\n";
  }
}

inline bool is_space(std::string const& str) noexcept {
  for (auto c : str) {
    if (!isspace(c)) return false;
  }
  return true;
}

inline bool is_comment(std::string const& str) noexcept {
  unsigned i{};
  for (; i < str.size() && isspace(str[i]); i++);
  return i < str.size() && str[i] == ';';
}

void interpret(std::string const& line) {
  if (is_space(line) || is_comment(line)) return;
  if (is_command(line)) interpret_command(line);
  else Asm::Interpreter::intepret_instruction(line);
}

void start_shell_mode() {
  std::cout << "Mini ASM version " + App::version << "\n"
    << "Created by Vincent P." << "\n"
    << "Shell mode - Type 'exit' to stop \n";
  std::string line;
  while (true) {
    try {
      std::cout << "> ";
      std::getline(std::cin, line);
      if (line == "exit") break;
      interpret(to_lower(line));
    } catch (std::exception const& e) {
      std::cout << std::string{"Error: "} + e.what();
    }
  }
}

std::string extract_jmp_token(std::string const& line) {
  unsigned i{};
  for (; i < line.size() && std::isspace(line[i]); i++);
  std::string token;
  for (; i < line.size() && !std::isspace(line[i]) && line[i] != ':'; i++) {
    token += line[i];
  }
  return to_lower(token);
}

void read_from_file(std::string const& filename) {
  std::ifstream file{filename};
  if (!file) {
    throw std::runtime_error{"Cannot open file " + filename};
  }
  std::string line;
  std::vector<std::string> code;
  // We need to use a vector because of (future) JMP instructions
  const std::regex jmp_token_regex{"[ \t]*[_a-zA-Z]*[_a-zA-Z0-9]+[ \t]*:"};
  unsigned i{};
  while (std::getline(file, line)) {
    if (std::regex_match(line, jmp_token_regex)) {
      auto token = extract_jmp_token(line);
      // If token already exists
      if (std::find_if(jmp_tokens.begin(), jmp_tokens.end(), [=](auto const& tok) -> bool { return token == tok.first; }) != jmp_tokens.end()) {
        throw std::runtime_error{"Multiple definitions of token " + token};
      } else {
        jmp_tokens.insert({token, i});
      }
      continue;
    }
    code.push_back(line.substr(0, line.find(';')));
    i++;
  }
  while (registers::PC < code.size()) {
    interpret(to_lower(code[registers::PC++]));
  }
}
