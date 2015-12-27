#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "cpu.h"
#include "stack.h"
#include "syntax.h"

/**
* @brief Swap the bytes of 2-bytes unsigned number
* @returns A new u16 with the first's bytes swapped
* @exception /
*/
constexpr u16 SwapBytes(u16 value) noexcept { // Not used yet
  return ((value & 0xff) << 8) + (value >> 8);
}

inline std::string to_lower(std::string const& str) {
  auto cpy = str;
	std::transform(cpy.begin(), cpy.end(), cpy.begin(), tolower);
  return cpy;
}

int main() {
  for (auto str : instructions) {
    std::cout << str << " ";
    bool match = false;
    for (auto regex : regexes) {
      if (std::regex_match(to_lower(str), regex)) {
        std::cout << "Ok" << std::endl;
        match = true;
      }
    }
    if (!match) {
      std::cout << "Not ok" << std::endl;
    }
  }
  std::cin.get();
}
