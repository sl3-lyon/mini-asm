#include "cpu.h"

#include <bitset>  // std::bitset
#include <sstream> // std::stringstream
#include <regex>   // std::regex, std::regex_match

Stack<0xff> stack{};

/**
 * @namespace registers
 * @brief Here are the registers used in MiniASM
 */
namespace registers {
	u8 A   = 0x00; //!< @brief Accumulator
	u8 X   = 0x00; //!< @brief X index
	u8 Y   = 0x00; //!< @brief Y index
	u8 P   = 0x00; //!< @brief Program status
	u16 PC = 0x00; //!< @brief Program counter
	u8 S   = 0x00; //!< @brief Stack pointer
} // namespace registers

/**
* @brief Buffer in which will be loader the ROM
*/
std::vector<u8> ROM(1000);

/**
* @brief RAM
*/
std::array<u8, 0xffff> RAM{};
unsigned ram_ptr = 0x100;

/**
* @brief Video RAM
*/
std::array<u8, 0xffff> VRAM; // Not used yet

/**
 * @brief Returns the next opcode
 * @pre PC register must be inferior to ROM size
 * @throw OutOfRangeException If the precondition is not satisfied
 * @see OutOfRangeException
 */
inline u8 read_next() {
	if (registers::PC >= ROM.size()) {
		throw Asm::Errors::OutOfRangeException{"Out of ROM file"};
	}
	return ROM[++registers::PC];
}

/**
 * @brief Reads next value as an address and returns the value stored at this address
 * @pre PC register must be inferior to ROM size
 * @throw OutOfRangeException If the precondition is not satisfied
 * @see OutOfRangeException
 */
inline u8 read_addr() {
	if (registers::PC >= ROM.size()) {
		throw Asm::Errors::OutOfRangeException{"Out of ROM file"};
	}
	return ROM[ROM[++registers::PC]];
}

/**
 * @brief Compares the arguments. Can set EQUAL, LOWER flag or GREATER flag
 * @throw /
 */
inline void cmp(u8 ra, u8 rb) noexcept {
	if (ra == rb) {
		registers::P |= Flags::equal;
		registers::P ^= Flags::lower;
		registers::P ^= Flags::greater;
	} else if (ra < rb) {
		registers::P |= Flags::lower;
		registers::P ^= Flags::equal;
		registers::P ^= Flags::greater;
	} else if (ra > rb) {
		registers::P |= Flags::greater;
		registers::P ^= Flags::equal;
		registers::P ^= Flags::lower;
	}
}

inline void cmp() {
	auto lop = read_next();
	auto rop = read_next();
	cmp(lop, rop);
}

//inline void cmpRegOp(u8 reg) {
//	auto op = read_next();
//	cmp(reg, op);
//}
//
//inline void cmpOpReg(u8 reg) {
//	auto op = read_next();
//	cmp(op, reg);
//}

inline void jmp(u16 addr) noexcept {
	registers::PC = ROM[addr];
}

inline void je(u16 addr) noexcept {
	if (registers::P & Flags::equal) {
		registers::PC = addr;
	}
}

inline void jne(u16 addr) noexcept {
	if (!(registers::P & Flags::equal)) {
		registers::PC = addr;
	}
}

inline void jl(u16 addr) noexcept {
	if (registers::P & Flags::lower) {
		registers::PC = addr;
	}
}

inline void jle(u16 addr) noexcept {
	if (registers::P & Flags::lower || registers::P & Flags::equal) {
		registers::PC = addr;
	}
}

inline void jg(u16 addr) noexcept {
	if (registers::P & Flags::greater) {
		registers::PC = addr;
	}
}

inline void jge(u16 addr) noexcept {
	if (registers::P & Flags::greater || registers::P & Flags::equal) {
		registers::PC = addr;
	}
}

inline void call(u8 value) {
	// TODO - Call
}

/**
 * @brief Checks if val is the name of a register
 * @param val The string we test
 * @returns True if val is the name of a register, false otherwise
 * @throw /
 */
inline bool is_register(std::string const& name) noexcept {
	return name == "A" || name == "X" || name == "Y"
		|| name == "P" || name == "S";
}

/**
 * @brief Checks if val is an address
 * @param val The string we test
 * @returns True if val is an address, false otherwise
 * @throw /
 */
inline bool is_address(std::string const& val) noexcept {
	return std::regex_match(val, std::regex{"\\*[0-9]+|\\*0x[0-9a-f]+|\\*0b[0-1]+"});
}

/**
 * @brief Returns a reference to one of the registers
 * @param name Name of a register
 * @pre name must be a name of a register
 * @throw std::runtime_error if name is not correct
 */
inline u8& get_register(std::string const& name) noexcept {
	if (name == "A") return registers::A;
	if (name == "X") return registers::X;
	if (name == "Y") return registers::Y;
	if (name == "S" || name == "P" || name == "PC") {
		throw std::runtime_error{"Cannot access to register '" + name + "'"};
	}
	throw std::runtime_error{"Invalid token '" + name + "'"};
}

inline u8 from_hex(std::string const& hex) {
	unsigned x;
	std::stringstream ss;
	ss << std::hex << hex.substr(2);
	ss >> x;
	return static_cast<u8>(x);
}

inline u8 from_bin(std::string const& bin) {
	return static_cast<u8>(std::bitset<8>(bin.substr(2)).to_ulong());
}

inline u8 get_value_of(std::string const& val) {
	// Register
	if (is_register(val)) {
		return get_register(val);
	}
	// Address
	if (is_address(val)) {
		if (std::regex_match(val, std::regex{"\\*[0-9]+"})) return static_cast<u8>(std::stoi(val.substr(1))); // Decimal
		if (std::regex_match(val, std::regex{"\\*0x[0-9a-f]+"}))  return from_hex(val.substr(3)); // Hex
		if (std::regex_match(val, std::regex{"\\*0b[0-1]+"}))  return from_bin(val.substr(3)); // Bin
	}
	// Simple value
	else {
		if (std::regex_match(val, std::regex{"[0-9]+"})) return static_cast<u8>(std::stoi(val)); // Decimal
		if (std::regex_match(val, std::regex{"0x[0-9a-f]+"})) return from_hex(val.substr(2)); // Hex
		if (std::regex_match(val, std::regex{"0b[0-1]+"})) return from_bin(val.substr(2)); // Bin
	}
	throw std::runtime_error{"Invalid token '" + val + "'"};
}


// TODO - Refactoring
void exec_mov(std::string const& param1, std::string const& param2) {
	if (is_register(param1)) {
		get_register(param1) = get_value_of(param2);
	} else if (is_address(param1)) {
		RAM[get_value_of(param1)] = get_value_of(param2);
	} else {
		throw std::runtime_error{"Cannot execute MOV"};
	}
}

void exec_add(std::string const& param1, std::string const& param2) {
	if (is_register(param1)) {
		get_register(param1) += get_value_of(param2);
	} else if (is_address(param1)) {
		RAM[get_value_of(param1)] += get_value_of(param2);
	} else {
		throw std::runtime_error{"Cannot execute ADD"};
	}
}

void exec_sub(std::string const& param1, std::string const& param2) {
	if (is_register(param1)) {
		get_register(param1) -= get_value_of(param2);
	} else if (is_address(param1)) {
		RAM[get_value_of(param1)] -= get_value_of(param2);
	} else {
		throw std::runtime_error{"Cannot execute SUB"};
	}
}

void exec_cmp(std::string const& param1, std::string const& param2) {
	auto val1 = get_value_of(param1);
	auto val2 = get_value_of(param2);
	if (val1 == val2) {
		registers::P |= Flags::equal;
	} else if (val1 > val2) {
		registers::P |= Flags::greater;
	} else if (val1 < val2) {
		registers::P |= Flags::lower;
	}
}

void exec_or(std::string const& param1, std::string const& param2) {
	if (is_register(param1)) {
		get_register(param1) |= get_value_of(param2);
	} else if (is_address(param1)) {
		RAM[get_value_of(param1)] |= get_value_of(param2);
	} else {
		throw std::runtime_error{"Cannot execute OR"};
	}
}

void exec_and(std::string const& param1, std::string const& param2) {
	if (is_register(param1)) {
		get_register(param1) &= get_value_of(param2);
	} else if (is_address(param1)) {
		RAM[get_value_of(param1)] &= get_value_of(param2);
	} else {
		throw std::runtime_error{"Cannot execute AND"};
	}
}

void exec_xor(std::string const& param1, std::string const& param2) {
	if (is_register(param1)) {
		get_register(param1) ^= get_value_of(param2);
	} else if (is_address(param1)) {
		RAM[get_value_of(param1)] ^= get_value_of(param2);
	} else {
		throw std::runtime_error{"Cannot execute XOR"};
	}
}

void exec_push(std::string const& param1) {
	stack.push(get_value_of(param1));
}

void exec_pop(std::string const& param1) {
	if (is_register(param1)) {
		get_register(param1) = stack.pop();
	} else if (is_address(param1)) {
		RAM[get_value_of(param1)] = stack.pop();
	}
}
// End TODO

void exec(std::string const& op, std::string const& param1, std::string const& param2) {
	if (op == "MOV") {
		exec_mov(param1, param2);
	} else if (op == "ADD") {
		exec_add(param1, param2);
	} else if (op == "SUB") {
		exec_sub(param1, param2);
	} else if (op == "CMP") {
		exec_sub(param1, param2);
	} else if (op == "OR") {
		exec_sub(param1, param2);
	} else if (op == "AND") {
		exec_sub(param1, param2);
	} else if (op == "XOR") {
		exec_sub(param1, param2);
	} else if (op == "PUSH") {
		exec_push(param1);
	} else if (op == "POP") {
		exec_pop(param1);
	}
}

std::map<u8, std::function<void(void)>> op = {
	{ 0x00, []() -> void {} },

	// --[ MOV ]--
	{ 0x01, []() -> void { registers::A = read_next(); } },      // mov A, VALUE
	{ 0x02, []() -> void { registers::A = RAM[read_next()]; } }, // mov A, ADDR
	{ 0x03, []() -> void { registers::X = read_next(); } },      // mov X, VALUE
	{ 0x04, []() -> void { registers::X = RAM[read_next()]; } }, // mov X, ADDR
	{ 0x05, []() -> void { registers::Y = read_next(); } },      // mov Y, VALUE
	{ 0x06, []() -> void { registers::Y = RAM[read_next()]; } }, // mov Y, ADDR

	// --[ ADD ]--
	{ 0x07, []() -> void { registers::A += read_next(); } },      // add A, VALUE
	{ 0x08, []() -> void { registers::A += RAM[read_next()]; } }, // add A, ADDR
	{ 0x09, []() -> void { registers::X += read_next(); } },      // add X, VALUE
	{ 0x0a, []() -> void { registers::X += RAM[read_next()]; } }, // add X, ADDR
	{ 0x0b, []() -> void { registers::Y += read_next(); } },      // add Y, VALUE
	{ 0x0c, []() -> void { registers::Y += RAM[read_next()]; } }, // add Y, ADDR

	// --[ SUB ]--
	{ 0x0d, []() -> void { registers::A -= read_next(); } },      // sub A, VALUE
	{ 0x0e, []() -> void { registers::A -= RAM[read_next()]; } }, // sub A, ADDR
	{ 0x0f, []() -> void { registers::X -= read_next(); } },      // sub X, VALUE
	{ 0x10, []() -> void { registers::X -= RAM[read_next()]; } }, // sub X, ADDR
	{ 0x11, []() -> void { registers::Y -= read_next(); } },      // sub Y, VALUE
	{ 0x12, []() -> void { registers::Y -= RAM[read_next()]; } }, // sub Y, ADDR

	// --[ SHL ]--
	{ 0x13, []() -> void { registers::A <<= read_next(); } }, // shl A, VALUE
	{ 0x14, []() -> void { registers::X <<= read_next(); } }, // shl X, VALUE
	{ 0x15, []() -> void { registers::Y <<= read_next(); } }, // shl Y, VALUE

	// --[ SHR ]--
	{ 0x16, []() -> void { registers::A >>= read_next(); } }, // shl A, VALUE
	{ 0x17, []() -> void { registers::X >>= read_next(); } }, // shl X, VALUE
	{ 0x18, []() -> void { registers::Y >>= read_next(); } }, // shl Y, VALUE

	// --[ CMP ]--
	{ 0x19, []() -> void { cmp(registers::A, registers::X); } }, // cmp A, X
	{ 0x1A, []() -> void { cmp(registers::A, registers::Y); } }, // cmp A, Y
	{ 0x1B, []() -> void { cmp(registers::A, read_next()); } }, // cmp A, op
	{ 0x1C, []() -> void { cmp(read_next(), registers::A); } }, // cmp op, A

	{ 0x1D, []() -> void { cmp(registers::X, registers::A); } }, // cmp X, A
	{ 0x1E, []() -> void { cmp(registers::X, registers::Y); } }, // cmp X, Y
	{ 0x1F, []() -> void { cmp(registers::X, read_next()); } }, // cmp X, op
	{ 0x20, []() -> void { cmp(read_next(), registers::X); } }, // cmp op, X

	{ 0x21, []() -> void { cmp(registers::Y, registers::A); } }, // cmp Y, A
	{ 0x22, []() -> void { cmp(registers::Y, registers::X); } }, // cmp Y, X
	{ 0x23, []() -> void { cmp(registers::Y, read_next()); } }, // cmp Y, op
	{ 0x24, []() -> void { cmp(read_next(), registers::Y); } }, // cmp op, Y

	// --[ JMP ]--
	{ 0x25, []() -> void { jmp(read_next()); } }, // jmp ADDR
	{ 0x26, []() -> void { je(read_next()); } },  // je ADDR
	{ 0x27, []() -> void { jne(read_next()); } }, // jne ADDR
	{ 0x28, []() -> void { jl(read_next()); } },  // jl ADDR
	{ 0x29, []() -> void { jle(read_next()); } }, // jle ADDR
	{ 0x2A, []() -> void { jg(read_next()); } },  // jg ADDR
	{ 0x2B, []() -> void { jge(read_next()); } }, // jge ADDR

	{ 0x2C, []() -> void { jmp(registers::A); } }, // jmp A
	{ 0x2D, []() -> void { je(registers::A); } },  // je A
	{ 0x2E, []() -> void { jne(registers::A); } }, // jne A
	{ 0x2F, []() -> void { jl(registers::A); } },  // jl A
	{ 0x30, []() -> void { jle(registers::A); } }, // jle A
	{ 0x31, []() -> void { jg(registers::A); } },  // jg A
	{ 0x32, []() -> void { jge(registers::A); } }, // jge A

	{ 0x33, []() -> void { jmp(registers::X); } }, // jmp X
	{ 0x34, []() -> void { je(registers::X); } },  // je X
	{ 0x35, []() -> void { jne(registers::X); } }, // jne X
	{ 0x36, []() -> void { jl(registers::X); } },  // jl X
	{ 0x37, []() -> void { jle(registers::X); } }, // jle X
	{ 0x38, []() -> void { jg(registers::X); } },  // jg X
	{ 0x39, []() -> void { jge(registers::X); } }, // jge X

	{ 0x3A, []() -> void { jmp(registers::Y); } }, // jmp Y
	{ 0x3B, []() -> void { je(registers::Y); } },  // je Y
	{ 0x3C, []() -> void { jne(registers::Y); } }, // jne Y
	{ 0x3D, []() -> void { jl(registers::Y); } },  // jl Y
	{ 0x3F, []() -> void { jg(registers::Y); } },  // jg Y
	{ 0x40, []() -> void { jge(registers::Y); } }, // jge Y

	// --[ CALL ]--
	{ 0x41, []() -> void { call(read_next()); } },  // call
	{ 0x42, []() -> void { call(registers::A); } }, // call

	// --[ PUSH ]--
	{ 0x43, []() -> void { stack.push(registers::A); } }, // push A
	{ 0x44, []() -> void { stack.push(registers::X); } }, // push X
	{ 0x45, []() -> void { stack.push(registers::Y); } }, // push Y
	{ 0x46, []() -> void { stack.push(registers::P); } }, // push P
	{ 0x47, []() -> void { stack.push(registers::S); } }, // push S

	// --[ POP ]--
	{ 0x48, []() -> void { registers::A = stack.pop(); } }, // pop A
	{ 0x49, []() -> void { registers::X = stack.pop(); } }, // pop X
	{ 0x4A, []() -> void { registers::Y = stack.pop(); } }, // pop Y

	// --[ OR ]--
	{ 0x4B, []() -> void { registers::A |= registers::A; } },     // or A, A
	{ 0x4C, []() -> void { registers::A |= registers::X; } },     // or A, X
	{ 0x4D, []() -> void { registers::A |= registers::Y; } },     // or A, Y
	{ 0x4E, []() -> void { registers::A |= read_next(); } },      // or A, VALUE
	{ 0x4F, []() -> void { registers::A |= RAM[read_next()]; } }, // or A, ADDR

	{ 0x50, []() -> void { registers::X |= registers::A; } },     // or X, A
	{ 0x51, []() -> void { registers::X |= registers::X; } },     // or X, X
	{ 0x52, []() -> void { registers::X |= registers::Y; } },     // or X, Y
	{ 0x53, []() -> void { registers::X |= read_next(); } },      // or X, VALUE
	{ 0x54, []() -> void { registers::X |= RAM[read_next()]; } }, // or X, ADDR

	{ 0x55, []() -> void { registers::Y |= registers::A; } },     // or Y, A
	{ 0x56, []() -> void { registers::Y |= registers::X; } },     // or Y, X
	{ 0x57, []() -> void { registers::Y |= registers::Y; } },     // or Y, Y
	{ 0x58, []() -> void { registers::Y |= read_next(); } },      // or Y, VALUE
	{ 0x59, []() -> void { registers::Y |= RAM[read_next()]; } }, // or Y, ADDR

	// --[ AND ]--
	{ 0x5A, []() -> void { registers::A &= registers::A; } },     // and A, A
	{ 0x5B, []() -> void { registers::A &= registers::X; } },     // and A, X
	{ 0x5C, []() -> void { registers::A &= registers::Y; } },     // and A, Y
	{ 0x5D, []() -> void { registers::A &= read_next(); } },      // and A, VALUE
	{ 0x5E, []() -> void { registers::A &= RAM[read_next()]; } }, // and A, ADDR

	{ 0x5F, []() -> void { registers::X &= registers::A; } },     // and X, A
	{ 0x61, []() -> void { registers::X &= registers::X; } },     // and X, X
	{ 0x62, []() -> void { registers::X &= registers::Y; } },     // and X, Y
	{ 0x63, []() -> void { registers::X &= read_next(); } },      // and X, VALUE
	{ 0x64, []() -> void { registers::X &= RAM[read_next()]; } }, // and X, ADDR

	{ 0x65, []() -> void { registers::Y &= registers::A; } },     // and Y, A
	{ 0x66, []() -> void { registers::Y &= registers::X; } },     // and Y, X
	{ 0x67, []() -> void { registers::Y &= registers::Y; } },     // and Y, Y
	{ 0x68, []() -> void { registers::Y &= read_next(); } },      // and Y, VALUE
	{ 0x69, []() -> void { registers::Y &= RAM[read_next()]; } }, // and Y, ADDR

	// --[ XOR ]--
	{ 0x6A, []() -> void { registers::A ^= registers::A; } },     // xor A, A
	{ 0x6B, []() -> void { registers::A ^= registers::X; } },     // xor A, X
	{ 0x6C, []() -> void { registers::A ^= registers::Y; } },     // xor A, Y
	{ 0x6D, []() -> void { registers::A ^= read_next(); } },      // xor A, VALUE
	{ 0x6E, []() -> void { registers::A ^= RAM[read_next()]; } }, // xor A, ADDR

	{ 0x6F, []() -> void { registers::X ^= registers::A; } },     // xor X, A
	{ 0x70, []() -> void { registers::X ^= registers::X; } },     // xor X, X
	{ 0x71, []() -> void { registers::X ^= registers::Y; } },     // xor X, Y
	{ 0x72, []() -> void { registers::X ^= read_next(); } },      // xor X, VALUE
	{ 0x73, []() -> void { registers::X ^= RAM[read_next()]; } }, // xor X, ADDR

	{ 0x74, []() -> void { registers::Y ^= registers::A; } },     // xor Y, A
	{ 0x75, []() -> void { registers::Y ^= registers::X; } },     // xor Y, X
	{ 0x76, []() -> void { registers::Y ^= registers::Y; } },     // xor Y, Y
	{ 0x77, []() -> void { registers::Y ^= read_next(); } },      // xor Y, VALUE
	{ 0x78, []() -> void { registers::Y ^= RAM[read_next()]; } }, // xor Y, ADDR

	{ 0x79, []() -> void { stack.push(read_next()); } },        // push VALUE
	{ 0x7A, []() -> void { stack.push(RAM[read_next()]); } },   // push ADDR
	{ 0x7B, []() -> void { RAM[read_next()] = stack.pop(); } }, // pop ADDR


	{ 0x7C, []() -> void { RAM[read_next()] = registers::A; } },     // mov ADDR, A
	{ 0x7D, []() -> void { RAM[read_next()] = registers::X; } },     // mov ADDR, X
	{ 0x7E, []() -> void { RAM[read_next()] = registers::Y; } },     // mov ADDR, Y
	{ 0x7F, []() -> void { RAM[read_next()] = read_next(); } },      // mov ADDR, VALUE
	{ 0x80, []() -> void { RAM[read_next()] = RAM[read_next()]; } }, // mov ADDR, ADDR

	{ 0x81, []() -> void { RAM[read_next()] += registers::A; } },     // add ADDR, A
	{ 0x82, []() -> void { RAM[read_next()] += registers::X; } },     // add ADDR, X
	{ 0x83, []() -> void { RAM[read_next()] += registers::Y; } },     // add ADDR, Y
	{ 0x84, []() -> void { RAM[read_next()] += read_next(); } },      // add ADDR, VALUE
	{ 0x85, []() -> void { RAM[read_next()] += RAM[read_next()]; } }, // add ADDR, ADDR

	{ 0x86, []() -> void { RAM[read_next()] -= registers::A; } },     // sub ADDR, A
	{ 0x87, []() -> void { RAM[read_next()] -= registers::A; } },     // sub ADDR, X
	{ 0x88, []() -> void { RAM[read_next()] -= registers::A; } },     // sub ADDR, Y
	{ 0x89, []() -> void { RAM[read_next()] -= read_next(); } },      // sub ADDR, VALUE
	{ 0x8A, []() -> void { RAM[read_next()] -= RAM[read_next()]; } }, // sub ADDR, ADDR
};