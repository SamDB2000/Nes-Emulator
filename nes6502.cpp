#include "nes6502.h"

nes6502::nes6502()
{
	// There is a 16x16 table for instructions. It's arranged so that the bottom
	// 4 bits of the instruction choose the column, and the top 4 bits choose the row.
	
	// The full populated matrix of instructions handled by the 6502.
	// Each member includes the pneumonic for the opcode,
	// function pointer for the opcode, function pointer for the address mode, and number of clock cycles.
	
	// This is a huge initializer list of initializer lists for each instruction

	using a = nes6502;
	lookup = {
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
	// Fun!
}

nes6502::~nes6502() {
	// Destructor - doesn't need anything
}

// Reads a single byte from the bus, located at the specified 16-bit address
uint8_t nes6502::read(uint16_t a)
{
	return bus->read(a, false);
}

// Writes a byte to the bus at the specified address
void nes6502::write(uint16_t a, uint8_t d)
{
	return bus->write(a, d);
}

//////////////////////////////////////////////////////////////////////////////
// External Inputs

// Forces the 6502 into a known state. Registers are set to 0x00,
// the status register is cleared except for unused bit. An absolute
// address is read from location 0xFFFC which contains a second
// address that the program counter is set to. This allows the
// programmer to jump to a known and programmable location in the
// memory to start executing from. Typically the programmer would
// set the value at location 0xFFFC at compile time.
void nes6502::reset()
{
	a = 0;
	x = 0;
	y = 0;
	stkp = 0xFD;
	status = 0x00 | U;

	addr_abs = 0xFFFC;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);

	pc = (hi << 8) | lo;

	addr_abs = 0x0000;
	addr_rel = 0x0000;
	fetched = 0x00;

	// Reset takes some time
	cycles = 8;
}

// Interrupt Request. Only occur if the "disable interrupts" flag is set to 0.
// IRQ's can happen at any time, but you don't want them to be destructive to
// the operation of the running program. Therefore, you wait for that operation
// to finish (which we do by waiting unitl cycles == 0) and then the current
// program counter is stored on the stack. Then the current status register
// is stored on teh stack. When the routne that services the interrupt is
// done, the status register and program register can be restored using the
// "RTI" instruction. Once the IRQ is done, a programmable address is read
// from location 0xFFFE, which is subsequently set to the program counter.
void nes6502::irq()
{
	if (GetFlag(I) == 0)
	{
		// Push pc to stack (takes two pushes since it is a 16-bit variable)
	}
}

void nes6502::nmi()
{

}

void nes6502::clock()
{
	if (cycles == 0) {
		opcode = read(pc);
		pc++;

		// Get Starting number of cycles
		cycles = lookup[opcode].cycles;

		// Use the opcode to call function required for the address mode
		uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();

		// Use the opcode to call the correct operation function
		uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

		// Each address and op function returns a 1 if they need an additional clock cycle, 0 otherwise
		cycles += (additional_cycle1 & additional_cycle2);
	}

	cycles--;
}

//////////////////////////////////////////////////////////////////////////////
// Flag Functions

//////////////////////////////////////////////////////////////////////////////
// Addressing Modes

uint8_t nes6502::IMP()
{
	fetched = a; // Set fetched variable to the contents of the accumulator
	return 0;
}

uint8_t nes6502::IMM()
{
	addr_abs = pc++;
	return 0;
}

// Zero page addressing
uint8_t nes6502::ZP0()
{
	addr_abs = read(pc);
	pc++;
	addr_abs &= 0x00FF; // Sets the page (the first byte of the memory address) to 0
	return 0;
}

// Zero page addressing with x register offset
uint8_t nes6502::ZPX()
{
	// Reads at a memory location plus x-offset (useful for iterating through memory - i.e. an array)
	addr_abs = (read(pc) + x);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

// Zero page addressing with y register offset
uint8_t nes6502::ZPY()
{
	// Reads at a memory location plus x-offset (useful for iterating through memory - i.e. an array)
	addr_abs = (read(pc) + y);
	pc++;
	addr_abs &= 0x00FF;
	return 0;
}

// Sets the absolute address
uint8_t nes6502::ABS()
{
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;

	return 0;
}

// Sets the absolute address with Y register offset
uint8_t nes6502::ABY()
{
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;
	addr_abs += y; // Adds y offset

	// If the address has changed to a different page after offset
	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

// Address Mode: Indirect (pointer)
/* The original hardware actually includes a bug. If the low byte of the supplied address is 0xFF,
 then to read the high byte of the actual address we need to cross a page boundary.
 This doesn't actually work on the chip, instead it wraps back around in the same
 page, yielding an invalid actual address. We have to implement this here.
 */
uint8_t nes6502::IND()
{
	// Supplied data is a pointer
	uint16_t ptr_lo = read(pc);
	pc++;
	uint16_t ptr_hi = read(pc);
	pc++;

	uint16_t ptr = (ptr_hi << 8) | ptr_lo;

	if (ptr_lo == 0x00FF) { // simulate the page boundary bug
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
	}
	else {
		// Set the asbolute address to the address that is the pointer
		addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
	}

	return 0;
}

// Address Mode: Indirect X
// The supplied 8-bit address is offset by X register to index
// a location in page 0x00. The actual 16-bit address is read
// from this location.
uint8_t nes6502::IZX()
{
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read((uint16_t)(t + (uint16_t)x) & 0x00FF);
	uint16_t hi = read((uint16_t)(t + (uint16_t)x + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;

	return 0;
}

// Address Mode: Indirect Y
// The supplied 8-bit address indexes a location in page 0x00. 
// The actual 16-bit address is read, and the contents of the Y register
// is added to it. If the offset causes a change in page then an
// clock cycle is required.
uint8_t nes6502::IZY()
{
	uint16_t t = read(pc);
	pc++;

	uint16_t lo = read(t & 0x00FF);
	uint16_t hi = read((t + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	addr_abs += y;

	if ((addr_abs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

// Address Mode: Relative
// Used in branches
uint8_t nes6502::REL()
{
	addr_rel = read(pc);
	pc++;

	// Check if first bit is a 1 (if it is signed)
	if (addr_rel & 0x80)
		addr_rel |= 0xFF00; // Set the high byte to all 1's
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Instructions

uint8_t nes6502::fetch()
{
	if (!(lookup[opcode].addrmode == &nes6502::IMP))
		fetched = read(addr_abs);
	return fetched;
}

// Add to the accumulator
// function: a += m + c
// Example of carry bit: a = 250, m = 10
// a + m = 6, c = 1
uint8_t nes6502::ADC()
{
	fetch();
	uint16_t result = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);
	SetFlag(C, result > 255);
	SetFlag(Z, (result & 0x00FF) == 0);
	SetFlag(N, result & 0x80);

	
	// My way of saying If the result has a different sign then both a and m
	/*if (a & 0x80 == fetched & 0x80) {
		if (result & 0x80 != a & 0x80) {
			SetFlag(V, 0x01);
		}
	}*/

	// Set the overflow bit based off of if a pos + pos = neg  or if  neg + neg = pos
	// Logical way of saying that if the result has a different sign (first bit) then both a and m
	SetFlag(V, (~( (uint16_t)a ^ (uint16_t)fetched ) & ( (uint16_t)a ^ (uint16_t)result )) & 0x0080);

	a = result & 0x00FF; // Load the 8-bit result into the accumulator

	return 1; // might return an extra clock cycle
}

// Subtract from the accumulator: a = a - m - (1 - c)
// Reorganized from the addition operation in order to save hardware:
// a = a + -(m - (1-c)) ->  a = a + -m + 1 + c
// 
// To make a signed number positive we need to take the 2's complement (invert bits and add 1)
// 
// Note that the equation used (1-c), but it got converted to + 1 + c.
// This is since we already have the +1, so all we need to do is invert the bites of the data
// therefore we can add the exact same way we did before.
uint8_t nes6502::SBC()
{
	fetch();

	// Invert the bottom 8 bits with bitwise xor
	uint16_t value = ((uint16_t)fetched) ^ 0x00FF;

	// Perform the exact same addition procedure
	uint16_t result = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);
	SetFlag(C, result > 255);
	SetFlag(Z, (result & 0x00FF) == 0);
	SetFlag(N, result & 0x80);
	SetFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)result)) & 0x0080);
	a = result & 0x00FF; // Load the 8-bit result into the accumulator
	return 1; // might return an extra clock cycle
}

// Bitwise logic AND
uint8_t nes6502::AND()
{
	fetch();
	a = a & fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}

// Branch if Carry Set
// Function: if(C == 1) pc = address
uint8_t nes6502::BCS()
{
	if (GetFlag(C) == 1) // If carry flag is set
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) // if page changes
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Equal
// Function:    if(Z == 1) pc = address
uint8_t nes6502::BEQ()
{
	if (GetFlag(Z) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Negative
// Function:    if(N == 1) pc = address
uint8_t nes6502::BMI()
{
	if (GetFlag(N) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Not Equal
// Function:    if(Z == 0) pc = address
uint8_t nes6502::BNE()
{
	if (GetFlag(Z) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Positive
// Function:    if(N == 0) pc = address
uint8_t nes6502::BPL()
{
	if (GetFlag(N) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Overflow Clear
// Function:    if(V == 0) pc = address
uint8_t nes6502::BVC()
{
	if (GetFlag(V) == 0)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Branch if Overflow Set
// Function:    if(V == 1) pc = address
uint8_t nes6502::BVS()
{
	if (GetFlag(V) == 1)
	{
		cycles++;
		addr_abs = pc + addr_rel;

		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Clear carry flag
uint8_t nes6502::CLC()
{
	SetFlag(C, false);
	return 0;
}

// Clear decimal flag
uint8_t nes6502::CLD()
{
	SetFlag(D, false);
	return 0;
}

// Clear Interrupt flag
uint8_t nes6502::CLC()
{
	SetFlag(I, false);
	return 0;
}

// Clear Overflow Flag
uint8_t nes6502::CLV()
{
	SetFlag(V, false);
	return 0;
}

// Push accumulator to stack
uint8_t nes6502::PHA()
{
	// 0x0100 is the hardcoded stack variable
	write(0x0100 + stkp, a);
	stkp--;
	return 0;
}

// Pop stack to accumulator
uint8_t nes6502::PLA() {
	stkp++;
	a = read(0x0100 + stkp);
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 0;
}


// Instruction: Set Carry Flag
// Function:    C = 1
uint8_t nes6502::SEC()
{
	SetFlag(C, true);
	return 0;
}


// Instruction: Set Decimal Flag
// Function:    D = 1
uint8_t nes6502::SED()
{
	SetFlag(D, true);
	return 0;
}


// Instruction: Set Interrupt Flag / Enable Interrupts
// Function:    I = 1
uint8_t nes6502::SEI()
{
	SetFlag(I, true);
	return 0;
}

uint8_t nes6502::XXX() {
	return 0;
}


//////////////////////////////////////////////////////////////////////////////
// Helper Functions

bool nes6502::complete()
{
	return cycles == 0;
}