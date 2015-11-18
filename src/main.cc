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
  explicit RomReader(std::string const& path) : file_(path), index_(0) {
    if (file_) {
      std::string line;
      while (std::getline(file_, line)) {
        buffer_ += line;
      }
      // TODO - Add header to ROM and ignore it here
    }
  }

  std::vector<u8> bytes() {
    std::vector<u8> bytes;
    while (index_ < buffer_.size()) {
      std::string sub;
      if (isdigit(buffer_[index_])) {
        sub += std::to_string(buffer_[index_] - '0');
      }
      else if (tolower(buffer_[index_]) >= 'a' && tolower(buffer_[index_]) <= 'f') {
        sub += buffer_[index_];
      }
      else {
        throw std::runtime_error{ "Unknown value " + buffer_[index_] };
      }
      index_++;
      if (isdigit(buffer_[index_])) {
        sub += std::to_string(buffer_[index_] - '0');
      }
      else if (tolower(buffer_[index_]) >= 'a' && tolower(buffer_[index_]) <= 'f') {
        sub += buffer_[index_];
      }
      else {
        throw std::runtime_error{ "Unknown value " + buffer_[index_] };
      }
      index_++;
      unsigned int x;
      std::stringstream ss;
      ss << std::hex << sub;
      ss >> x;
      bytes.push_back(static_cast<u8>(x));
    }
    return bytes;
  }

private:
  unsigned index_;
  std::ifstream file_;
  std::string buffer_;
};

int main()
{
  std::string path = "lel.txt";
  RomReader reader{path};
  auto bytes = reader.bytes();
  std::cout << int(bytes[0]) << std::endl
    << int(bytes[1]) << std::endl
    << int(bytes[2]) << std::endl
    << int(bytes[3]) << std::endl;
  std::cin.get();
}
