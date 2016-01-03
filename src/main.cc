#include <fstream>
#include <iostream>
#include <sstream>

#include "cpu.h"
#include "stack.h"
#include "syntax.h"
#include "interpreter.h"

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

#include <regex>

const std::vector<std::regex> regexes = {
	std::regex{ "mov[ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*" },
	std::regex{ "add[ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*" },
	std::regex{ "sub[ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*" },
	std::regex{ "cmp[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*" },
	std::regex{ "or[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*" },
	std::regex{ "and[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*" },
	std::regex{ "xor[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*" },
	std::regex{ "push[ \t]+([a|x|y|p|s]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+|'[a-zA-Z0-9]')[ \t]*" },
	std::regex{ "pop([ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+))?[ \t]*" },
	std::regex{ "(jmp|je|jne|jl|jle|jg|jge)[ \t]+([a|x|y]|[0-9]+|0x[0-9a-f]+|0b[0-1]+)[ \t]*" }
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

inline std::string to_lower(std::string const& str) {
	auto cpy = str;
	for (auto& c : cpy) {
		c = tolower(c);
	}
	return cpy;
}

#include <sstream>

int main()
{
	/*std::string path = "lel.txt";
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
	}*/
	/*auto bytes = reader.bytes();
	std::cout << int(bytes[0]) << std::endl
		<< int(bytes[1]) << std::endl
		<< int(bytes[2]) << std::endl
		<< int(bytes[3]) << std::endl;*/
	//auto bytes = reader.bytes();
	/*std::cout << "A: " << int(registers::A) << std::endl
		<< "X: " << int(registers::X) << std::endl
		<< "Y: " << int(registers::Y) << std::endl
		<< "PC: " << int(registers::PC) << std::endl;*/

	//std::string mov1 = "mov a, 0b1110001";
	//std::string mov2 = "mov	a	,	*42";
	//std::string mov3 = "mov a,0x42";
	//std::string push1 = "push 'a'";
	//std::string push2 = "push		0xff";
	//std::string push3 = "push	0b1101";
	//std::string pop1 = "pop *0xff";
	//std::string pop2 = "pop		*0b0001";
	//std::string pop3 = "pop		a";
	//std::regex move_regex{"mov[ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*"};
	//std::regex add_regex{ "add[ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*" };
	//std::regex sub_regex{ "sub[ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*" };
	//std::regex cmp_regex{ "cmp[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*" };
	//std::regex or_regex{ "or[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*" };
	//std::regex and_regex{ "and[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*" };
	//std::regex xor_regex{ "xor[ \t]+[a|x|y][ \t]*,[ \t]*([a|x|y]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+)[ \t]*" };
	//std::regex push_regex{ "push[ \t]+([a|x|y|p|s]|0b[0-1]+|0x[0-9a-f]+|[0-9]+|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+|'[a-zA-Z0-9]')[ \t]*" };
	//std::regex pop_regex{ "pop([ \t]+([a|x|y]|\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+))?[ \t]*" };
	///*std::cout << std::boolalpha << std::regex_match(push1, push_regex)
	//	<< std::regex_match(pop2, pop_regex)
	//	<< std::regex_match(pop3, pop_regex);*/
	//for (auto str : instructions)
	//{
	//	std::cout << str << " ";
	//	bool match = false;
	//	for (auto regex : regexes) {
	//		if (std::regex_match(to_lower(str), regex)) {
	//			std::cout << "Ok" << std::endl;
	//			match = true;
	//		}
	//	}
	//	if (!match) {
	//		std::cout << "Not ok" << std::endl;
	//	}
	//}
	/// Test with asm line
	std::string line = " mov		A,	250   ;lol";
	line = line.substr(0, line.find(';'));
	Asm::Interpreter::intepret_instruction("MOV x, 42");
	Asm::Interpreter::intepret_instruction("push x");
	Asm::Interpreter::intepret_instruction("push 255");
	Asm::Interpreter::intepret_instruction("pop a");
	Asm::Interpreter::intepret_instruction("pop x");
	/*line = line.substr(0, line.find("/"));
	std::cout << "Inst: '" << Asm::Syntax::extract_param2(line) << "'" << std::endl;*/
	std::cout << "Register A = " << (int)registers::A << std::endl
		<< "Register X = " << (int)registers::X << std::endl;
	std::cin.get();
}

