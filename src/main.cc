#include <fstream>
#include <iostream>
#include <sstream>

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

int main(int argc, char **argv) {
  if (argc == 1) {
    start_shell_mode();
  } else if (argc == 2) {
    // TODO - Read file
    (void)argv;
  } else {
    std::cout << "Wrong number of arguments: expected file name or no argument for shell mode";
  }
}

const std::vector<std::regex> command_regexes = {
  std::regex{"(p r|print register) (a|x|y|p|s|pc)"},
  std::regex{"(p r|print registers)"}
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
  if (command == "p r" || command == "print registers") print_registers();
  else if (std::regex_match(command, std::regex{"p r (a|x|y|p|s|pc)"})) {
    std::cout << static_cast<unsigned>(get_register(command.substr(4))) << "\n";
  } else if (std::regex_match(command, std::regex{"print register (a|x|y|p|s|pc)"})) {
    std::cout << static_cast<unsigned>(get_register(command.substr(15))) << "\n";
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
    << "Shell mode - Type  'exit' to stop \n";
  std::string line;
  while (line != "exit") {
    try {
      std::cout << "> ";
      std::getline(std::cin, line);
      interpret(to_lower(line));
    } catch (std::exception const& e) {
      std::cout << std::string{"Error: "} + e.what();
    }
  }
}
