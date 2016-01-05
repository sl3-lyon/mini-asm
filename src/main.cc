#include <fstream>
#include <iostream>
#include <sstream>

#include "cpu.h"
#include "infos.h"
#include "interpreter.h"

/**
* @brief Swap the bytes of 2-bytes unsigned number
* @returns A new u16 with the first's bytes swapped
* @exception /
*/
constexpr u16 SwapBytes(u16 value) noexcept { // Not used yet
  return ((value & 0xff) << 8) + (value >> 8);
}

// Not used yet
class RomReader {
public:
  explicit RomReader(std::string const& path) : file_(path) {
    if (file_) {
      std::string line;
      while (std::getline(file_, line)) {
        buffer_ += line;
      }
      // TODO - Add header to ROM and ignore it here
    }
  }

  std::vector<u8> bytes() const {
    std::vector<u8> bytes;
    auto buffer = buffer_;
    unsigned index{};
    while (index < buffer.size()) {
      std::string sub;
      if (isdigit(buffer[index])) {
        sub += std::to_string(buffer[index] - '0');
      } else if (tolower(buffer[index]) >= 'a' && tolower(buffer[index]) <= 'f') {
        sub += buffer[index];
      } else {
        throw std::runtime_error{ "Unknown value " + buffer[index] };
      }
      index++;
      if (isdigit(buffer[index])) {
        sub += std::to_string(buffer[index] - '0');
      } else if (tolower(buffer[index]) >= 'a' && tolower(buffer[index]) <= 'f') {
        sub += buffer[index];
      } else {
        throw std::runtime_error{ "Unknown value " + buffer[index] };
      }
      index++;
      unsigned int x;
      std::stringstream ss;
      ss << std::hex << sub;
      ss >> x;
      bytes.push_back(static_cast<u8>(x));
    }
    return bytes;
  }

private:
  std::ifstream file_;
  std::string buffer_;
};

void start_shell_mode();

int main(int argc, char **argv) {
  if (argc == 1) {
    start_shell_mode();
  } else if (argc == 2) {
    // TODO - Read file
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
  do {
    try {
      std::cout << "> ";
      std::getline(std::cin, line);
      interpret(line);
    } catch (std::exception const& e) {
      std::cout << std::string{"Error: "} + e.what();
    }
  } while (line != "exit");
}
