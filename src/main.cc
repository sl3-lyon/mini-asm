#include <fstream>
#include <iostream>
#include <sstream>

#include "cpu.h"
#include "stack.h"

/**
* @brief Swap the bytes of 2-bytes unsigned number
* @returns A new u16 with the first's bytes swapped
* @exception /
*/
constexpr u16 SwapBytes(u16 value) noexcept {
  return ((value & 0xff) << 8) + (value >> 8);
}

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

int main()
{
  std::string path = "lel.txt";
  RomReader reader{path};
  ROM = reader.bytes();
  std::cout << "PC: " << int(registers::PC) << std::endl;
  for (; registers::PC < ROM.size(); registers::PC++) {
    auto const byte = ROM[registers::PC];
    if (op.find(byte) == op.end()) {
      std::cout << "Unknown op " << byte;
      break;
    }
    auto fun = op[byte];
    fun();
  }
  /*auto bytes = reader.bytes();
  std::cout << int(bytes[0]) << std::endl
    << int(bytes[1]) << std::endl
    << int(bytes[2]) << std::endl
    << int(bytes[3]) << std::endl;*/
  auto bytes = reader.bytes();
  std::cout << "A: " << int(registers::A) << std::endl
    << "X: " << int(registers::X) << std::endl
    << "Y: " << int(registers::Y) << std::endl
    << "PC: " << int(registers::PC) << std::endl;
  std::cin.get();
}
