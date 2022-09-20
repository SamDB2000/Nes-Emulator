#include "nes6502.h"
#include "Bus.h"

nes6502::nes6502() {
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
	// Gross blehhhh, I just copy pasted this
}

nes6502::~nes6502() {
	// Destructor - doesn't need anything
}

// Reads a single byte from the bus, located at the specified 16-bit address
uint8_t nes6502::read(uint16_t addr)
{
	// The reason that this is false is because cpuRead will change the
	// state of some devices on the bus. However we don't want this to
	// happen when we run the disassembler.
	return bus->cpuRead(addr, false);
}

// Writes a byte to the bus at the specified address
void nes6502::write(uint16_t addr, uint8_t data)
{
	bus->cpuWrite(addr, data);
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
	// For simple testing this can be changed
	// This sets the program counter
	addr_abs = 0xFFFC;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);
	pc = (hi << 8) | lo;

	a = 0;
	x = 0;
	y = 0;
	stkp = 0xFD;
	status = 0x00 | U; // | I;
	
	// COMMENT THIS WHEN NOT TESTING CPU USING NESTEST
	// pc = 0xC000;

	// Clear internal helper variables
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
// is stored on the stack. When the routine that services the interrupt is
// done, the status register and program register can be restored using the
// "RTI" instruction. Once the IRQ is done, a programmable address is read
// from location 0xFFFE, which is subsequently set to the program counter.
void nes6502::irq()
{
	if (GetFlag(I) == 0)
	{
		// Push program counter to stack (takes two pushes since it is a 16-bit variable)
		write(0x0100 + stkp, (pc >> 8) & 0x00FF);
		stkp--;
		write(0x0100 + stkp, pc & 0x00FF);
		stkp--;

		// Write the status register to the stack
		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(I, 1);
		write(0x0100 + stkp, status);
		stkp--;

		// Jump to a location for the program to handle an interrupt
		addr_abs = 0xFFFE;
		uint16_t lo = read(addr_abs + 0);
		uint16_t hi = read(addr_abs + 1);
		pc = (hi << 8) | lo;

		cycles = 7;
	}
}

// Non maskable interrupt:
// The only difference between the IRQ function is that it is unignorable
// and the program counter address is read from location 0xFFFA.
void nes6502::nmi()
{
	// Push pc to stack (takes two pushes since it is a 16-bit variable)
	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	SetFlag(B, 0);
	SetFlag(U, 1);
	SetFlag(I, 1);
	write(0x0100 + stkp, status);
	stkp--;

	addr_abs = 0xFFFA;
	uint16_t lo = read(addr_abs + 0);
	uint16_t hi = read(addr_abs + 1);
	pc = (hi << 8) | lo;

	cycles = 8;
}

void nes6502::clock()
{
	if (cycles == 0) {

		// Read the next instruction byte
		// This 8-bit value is used to index the translation table to get
		// the relevant information about how to implement the instruction
		opcode = read(pc);

#ifdef LOGMODE
		uint16_t log_pc = pc;
#endif

		// Set unused status flag bit
		SetFlag(U, true);

		pc++;

		// Get Starting number of cycles
		cycles = lookup[opcode].cycles;

		// Use the opcode to call function required for the address mode
		uint8_t additional_cycle1 = (this->*lookup[opcode].addrmode)();

		// Use the opcode to call the correct operation function
		uint8_t additional_cycle2 = (this->*lookup[opcode].operate)();

		// Each address and op function returns a 1 if they need an additional clock cycle, 0 otherwise
		cycles += (additional_cycle1 & additional_cycle2);

		SetFlag(U, true);

#ifdef LOGMODE
		// This logger dumps the processor state every cycle.
		// Used for debugging only, as it is very slow.
		if (logfile == nullptr) 
			fopen_s(&logfile, "nes6502.txt", "wt");
		if (logfile != nullptr) {
			fprintf(logfile, "%04X                              A:%02X X:%02X Y:%02X P:%02X SP:%02X\n",
				log_pc, a, x, y, status, stkp);
		}
#endif
	}

	clock_count++;

	cycles--;
}

//////////////////////////////////////////////////////////////////////////////
// Flag Functions

// Returns the value of a specific bit of the status register
uint8_t nes6502::GetFlag(FLAGS6502 f) {
	return ((status & f) > 0 ? 1 : 0);
}

// Sets or clears a specific bit of the status register
void nes6502::SetFlag(FLAGS6502 f, bool v) {
	if (v) {
		status |= f;
	}
	else {
		status &= ~f;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Addressing Modes

uint8_t nes6502::IMP()
{
	// Set fetched variable to the contents of the accumulator
	fetched = a; 
	return 0;
}

uint8_t nes6502::IMM()
{
	// The data will be in the next byte of the program
	addr_abs = pc++;
	return 0;
}

// Zero page addressing
// This is used to save program bytes, since it only requires 1 byte
// for an address within the first page.
uint8_t nes6502::ZP0()
{
	addr_abs = read(pc);
	pc++;
	// Sets the page (the first byte of the memory address) to 0
	addr_abs &= 0x00FF;
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

// Sets the absolute address with X register offset
uint8_t nes6502::ABX()
{
	uint16_t lo = read(pc);
	pc++;
	uint16_t hi = read(pc);
	pc++;

	addr_abs = (hi << 8) | lo;
	addr_abs += x; // Adds x offset

	// If the address has changed to a different page after offset
	if ((addr_abs & 0xFF00) != (hi << 8)) {
		return 1;
	}
	else {
		return 0;
	}
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
	if ((addr_abs & 0xFF00) != (hi << 8)) {
		return 1;
	}
	else {
		return 0;
	}
}

// Address Mode: Indirect (pointer)
/* The original hardware actually includes a bug. If the low byte of the 
 supplied address is 0xFF, then to read the high byte of the actual address
 we need to cross a page boundary.
 This doesn't actually work on the chip, instead it wraps back around in the 
 same page, yielding an invalid address. We have to implement this here.
 */
// The supplied 16-bit address is read to get the actual 16-bit address
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
		// Set the asbolute address to the address supplied by the ptr
		addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
	}

	return 0;
}

// Address Mode: Indirect X
// The supplied 8-bit address is offset by the X register to index
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

// Address Mode: Indirect Zero Page with Y-offset
// The supplied 8-bit address indexes a location in page 0x00. 
// The residing 16-bit address is read, and the contents of the Y register
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
	// The wiki says that the pc gets incremented twice,
	// but source code doesn't have this. 
	// pc++;
	// After trying, it only messes things up... curious

	// Check if first bit is a 1 (if it it's signed)
	if (addr_rel & 0x80)
		addr_rel |= 0xFF00; // Set the high byte to all 1's
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Instructions

uint8_t nes6502::fetch()
{
	// Don't need to fetch if in the Implied Addressing Mode
	if (!(lookup[opcode].addrmode == &nes6502::IMP))
		fetched = read(addr_abs);
	return fetched;
}

// Add to the accumulator
// function: a = a + m + c
// Example of carry bit: a = 250, m = 10
// a + m = 4, c = 1
uint8_t nes6502::ADC()
{
	fetch();
	// res = a + m + c
	// This is done as a 16bit operation to make flag setting simpler
	temp = (uint16_t)a + (uint16_t)fetched + (uint16_t)GetFlag(C);
	SetFlag(C, temp > 255);
	SetFlag(Z, (temp & 0x00FF) == 0);
	SetFlag(N, temp & 0x80);

	// Set the overflow bit based off of if a pos + pos = neg  or if  neg + neg = pos
	// Logical way of saying that if the result has a different sign (first bit) then both a and m
	SetFlag(V, (~((uint16_t)a ^ (uint16_t)fetched) & ((uint16_t)a ^ (uint16_t)temp)) & 0x0080);

	a = temp & 0x00FF; // Load the 8-bit result into the accumulator

	return 1; // might return an extra clock cycle
}

// Bitwise logic AND between the accumulator and the fetched data
uint8_t nes6502::AND()
{
	fetch();
	a = a & fetched;
	// Set the Zero flag if the accumulator is Zero
	SetFlag(Z, a == 0x00);
	// Set the negative flag if the 7th bit is 1
	SetFlag(N, a & 0x80);
	return 1;
}

// Shift Left One Bit (Memory or Accumulator)
// A = C <- (A << 1) <- 0
// Flags out: N, Z, C
uint8_t nes6502::ASL()
{
	fetch();
	// Left shift 16 bit temp variable
	temp = (uint16_t)fetched << 1;
	SetFlag(C, (temp & 0xFF00) > 0);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x80);

	// If IMP address mode, change accumulator
	if (lookup[opcode].addrmode == &nes6502::IMP)
		a = temp & 0x00FF;
	else
		write(addr_abs, temp & 0x00FF); // Write in memory
	return 0;
}

// Branch on Carry Clear
// Function: if(C == 0) pc = address
uint8_t nes6502::BCC()
{
	if (GetFlag(C) == 0) {
		cycles++;
		addr_abs = pc + addr_rel;

		// if page changes
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) 
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Branch if Carry bit of the status register is set
// Function: if(C == 1) pc = address
uint8_t nes6502::BCS()
{
	if (GetFlag(C) == 1) // If carry flag is set
	{
		cycles++;
		addr_abs = pc + addr_rel;

		// if page changes, add a cycle
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) 
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

// Test Bits in Memory with Accumulator
// Tests if one or more bits are set in a target memory location
// The mask pattern in a is &'ed with the value in memory to set or clear Z flag
// Bits 7 and 6 of the value from memory are copied into the N and V flags
uint8_t nes6502::BIT()
{
	fetch();
	temp = a & fetched;
	SetFlag(Z, temp == 0x00);
	SetFlag(N, fetched & 0x80);
	SetFlag(V, fetched & 0x40);
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
		// Add a cycle upon successful branch
		cycles++;
		addr_abs = pc + addr_rel;

		// Add a cycle if branching to a new page
		if ((addr_abs & 0xFF00) != (pc & 0xFF00))
			cycles++;

		pc = addr_abs;
	}
	return 0;
}

// Instruction: Force Break
// Function: Forces the generation of an interrupt request.
// The pc and statuses are pushed on the stack
uint8_t nes6502::BRK() {
	pc++;

	SetFlag(I, 1);
	// 0x0100 is the hardcoded location the stack starts
	// Write the top 8 bits of the PC the stack
	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	// Write the bottom 8 of the pc to the stack
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	// Set the break flag to 1 before writing to stack
	SetFlag(B, 1);
	// Write the status register to the stack
	write(0x0100 + stkp, status);
	stkp--;
	SetFlag(B, 0);

	// IRQ interrupt vector at 0xFFFE/F is loaded into the PC
	pc = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
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
uint8_t nes6502::CLI()
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

// Instruction: CMP - Compare Memory and Accumulator
// Function: Change flags based on fetched data and accumulator Z,C,N = A - M
uint8_t nes6502::CMP() {
	fetch();
	temp = (uint16_t)a - (uint16_t)fetched;
	SetFlag(C, a >= fetched);
	SetFlag(Z, a == fetched);
	SetFlag(N, temp & 0x0080);
	return 1;
}

// Instruction: CPX - Compare Memory and X Register
// Function: Change C, Z, N, flags based on fetched data and X Register
uint8_t nes6502::CPX() {
	fetch();
	temp = (uint16_t)x - (uint16_t)fetched;
	SetFlag(C, x >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

// Instruction: CPY - Compare Memory and Y Register
// Function: Change C, Z, N, flags based on fetched data and Y Register
uint8_t nes6502::CPY() {
	fetch();
	temp = (uint16_t)y - (uint16_t)fetched;
	SetFlag(C, y >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

// Instruction: DEC - Decrement Memory
// Function: M, Z, N = M - 1
// Sets Z and N flags appropriately
uint8_t nes6502::DEC() {
	fetch();
	temp = fetched - 0x01;
	// Make sure to overwrite the data in memory
	write(addr_abs, temp & 0x00FF);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 0;
}

// Instruction: DEX - Decrement X Register
// Function: X, Z, N = X - 1
// Sets Z and N flags appropriately
uint8_t nes6502::DEX() {
	x--;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}

// Instruction: DEY - Decrement Y Register
// Function: Y, Z, N = Y - 1
// Sets Z and N flags appropriately
uint8_t nes6502::DEY() {
	y--;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}

// Instruction: EOR - "Exclusive-OR" Memory with Accumulator
// Function: A ^ M -> A
// Add a cycle if a page boundary is crossed
uint8_t nes6502::EOR() {
	fetch();
	a = a ^ fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}

// Instruction: Incrememnt Memory
uint8_t nes6502::INC() {
	fetch();
	fetched = fetched + 0x01;
	write(addr_abs, fetched);
	SetFlag(Z, fetched == 0x00);
	SetFlag(N, fetched & 0x80);
	return 0;
}

// Instruction: Increment X Register
uint8_t nes6502::INX() {
	x++;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}

// Instruction: Increment Y Register
uint8_t nes6502::INY() {
	y++;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}

// Instruction: JMP - Jump
// Function: Jump to New Location. Sets the program counter to the
// address specified by the operand. (No flag changes)
// JMP is an Absolute Indirect Addressing Mode (ABS)
uint8_t nes6502::JMP() {
	pc = addr_abs;
	return 0;
}

// Instruction: JSR - Jump to Subroutine
// Function: Pushes the current program counter (minus one) on to
// the stack and then sets the program counter to the target memory address.
// (No flag changes)
uint8_t nes6502::JSR() {
	pc--;
	write(0x0100 + stkp, (pc >> 8) & 0x00FF);
	stkp--;
	write(0x0100 + stkp, pc & 0x00FF);
	stkp--;

	pc = addr_abs;
	return 0;
}

// Instruction: LDA - Load Accumulator
// Function: Loads a byte of memory into the accumulator
// A, Z, N = M
uint8_t nes6502::LDA() {
	fetch();
	a = fetched;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 1;
}

// Instruction: LDX - Load X Register
// Function: Loads a byte of memory into the X Register
// X, Z, N = M
uint8_t nes6502::LDX() {
	fetch();
	x = fetched;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 1;
}

// Instruction: LDY - Load Y Register
// Function: Loads a byte of memory into the Y Register
// Y, Z, N = M
uint8_t nes6502::LDY() {
	fetch();
	y = fetched;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 1;
}

// Instruction: LSR - Logical Shift Right
// Function: A,C,Z,N = A/2 or M,C,Z,N = M/2
// Each of the bits in A or M is shifted one place to the right.
// The bit that was in the bit 0 is shifted onto the carry flag.
uint8_t nes6502::LSR() {
	fetch();
	SetFlag(C, (uint16_t)fetched & 0x0001);
	// Right shift 8 bit temp variable
	temp = (uint16_t)fetched >> 1;
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);

	// If IMP address mode, change accumulator
	if (lookup[opcode].addrmode == &nes6502::IMP) {
		a = temp & 0x00FF;
	}
	else {
		write(addr_abs, temp & 0x00FF); // Write in memory
	}

	return 0;

}

uint8_t nes6502::NOP()
{
	// Sadly not all NOPs are equal, I've added a few here
	// based on https://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
	// and will add more based on game compatibility, and ultimately
	// I'd like to cover all illegal opcodes too
	switch (opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}


// Bitwise logic incluse OR between the accumulator and the fetched data
uint8_t nes6502::ORA()
{
	fetch();
	a = a | fetched;
	// Set the Zero flag if the accumulator is Zero
	SetFlag(Z, a == 0x00);
	// Set the negative flag if the 7th bit is 1
	SetFlag(N, a & 0x80);
	return 1;
}

// Push accumulator to stack
uint8_t nes6502::PHA()
{
	// 0x0100 is the hardcoded stack variable
	write(0x0100 + stkp, a);
	stkp--; // decrease the stack pointer
	return 0;
}

// Instruction: Push Processor Status
// Pushes a copy of status flags onto the stack
// Note: Break flag is set to 1 before push
uint8_t nes6502::PHP() {
	// TODO: This one looks correct based off the guide
	// but source code is different, check olcNES if not working
	write(0x0100 + stkp, status | B | U);
	SetFlag(B, 0);
	SetFlag(U, 0);
	stkp--;
	return 0;
}

// Pop stack to accumulator
uint8_t nes6502::PLA() {
	stkp++; // incremenent stack pointer
	a = read(0x0100 + stkp); // read from the stack on the accumulator
	SetFlag(Z, a == 0x00);   // Set Z if zero
	SetFlag(N, a & 0x80);    // Negative if highest bit is 1
	return 0;
}

// Instruction: PLP - Pull Processor Status
// Function: Pulls an 8 bit value from the stack and into the processor flags.
// The flags will take on new states as determined by the value pulled.
uint8_t nes6502::PLP() {
	stkp++;
	status = read(0x0100 + stkp);
	SetFlag(U, 1);
	return 0;
}

// Instruction: ROL - Rotate Left
// Move each of the bits in either A or M one place to the left.
// Bit 0 is filled with the carry flag while the old bit 7 replaces carry flag.
uint8_t nes6502::ROL() {
	fetch();
	// Left shift by 1
	temp = fetched << 1;
	// Add the carry flag to the end
	// This assumes the 0 bit's value is 0 after a left shift
	temp += GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);

	// If IMP address mode, change to accumulator
	if (lookup[opcode].addrmode == &nes6502::IMP) {
		a = temp & 0x00FF;
	}
	else {
		write(addr_abs, temp & 0x00FF); // Write in memory
	}

	return 0;
}

// ROR - Rotate Right
// Move each of the bits in either A or M one place to the right.
// Bit 7 is filled with the current value of the carry flag 
// while the old bit 0 becomes the new carry flag value.
uint8_t nes6502::ROR() {
	fetch();
	temp = fetched >> 1;
	temp |= GetFlag(C) << 7;
	SetFlag(C, fetched & 0x01);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x0080);

	// If IMP address mode, change to accumulator
	if (lookup[opcode].addrmode == &nes6502::IMP) {
		a = temp & 0x00FF;
	}
	else {
		write(addr_abs, temp & 0x00FF); // Write in memory
	}

	return 0;
}

// A return after interrupt function. Reverts status and program counter
// by reading off of the stack. Called after an interrupt request.
uint8_t nes6502::RTI()
{
	stkp++;
	status = read(0x0100 + stkp);
	status &= ~B;
	status &= ~U;

	stkp++;
	pc = (uint16_t)read(0x0100 + stkp);
	stkp++;
	pc |= (uint16_t)read(0x0100 + stkp) << 8;
	return 0;
}

// RTS - Return from Subroutine
// The RTS instruction is used at the end of a subroutine to return to the calling routine.
// It pulls the program counter (minus one) from the stack.
uint8_t nes6502::RTS() {
	stkp++;
	pc = (uint16_t)read(0x0100 + stkp);
	stkp++;
	pc |= (uint16_t)read(0x0100 + stkp) << 8;

	pc++;
	return 0;
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
	temp = (uint16_t)a + value + (uint16_t)GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, ((temp & 0x00FF) == 0));
	SetFlag(N, temp & 0x0080);
	SetFlag(V, (temp ^ (uint16_t)a) & (temp ^ value) & 0x0080);
	a = temp & 0x00FF; // Load the 8-bit result into the accumulator
	return 1; // might return an extra clock cycle
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

// STA - Store Accumulator
// Stores the contents of the accumulator into memory
uint8_t nes6502::STA() {
	write(addr_abs, a);
	return 0;
}

// STX - Store X Register
// Stores the contents of the X Register into memory
uint8_t nes6502::STX() {
	write(addr_abs, x);
	return 0;
}

// STY - Store Y Register
// Stores the contents of the Y Register into memory
uint8_t nes6502::STY() {
	write(addr_abs, y);
	return 0;
}

// TAX - Transfer Accumulator to X
// Copies the contents of the accumulator into the X register
// Sets zero and negative flags
uint8_t nes6502::TAX() {
	x = a;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}

// TAY - Transfer Accumulator to Y
// Copies the contents of the accumulator into the Y register
// Sets zero and negative flags
uint8_t nes6502::TAY() {
	y = a;
	SetFlag(Z, y == 0x00);
	SetFlag(N, y & 0x80);
	return 0;
}

// TSX - Transfer Stack Pointer to X
// Copies the current contents of the stack register into the X register
// Sets zero and negative flags accordingly
uint8_t nes6502::TSX() {
	x = stkp;
	SetFlag(Z, x == 0x00);
	SetFlag(N, x & 0x80);
	return 0;
}

// TXA - Transfer X to Accumulator
// Copies the current contents of the X register into the accumulator
// Sets zero and negative flags accordingly
uint8_t nes6502::TXA() {
	a = x;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
	return 0;
}

// TXS - Transfer X to Stack Pointer
// Copies the current contents of the X register into the stack pointer
// Sets zero and negative flags accordingly
uint8_t nes6502::TXS() {
	stkp = x;
	return 0;
}

// TYA - Transfer Y Register to Accumulator
// Copies the current contents of the Y register into the accumulator
// Sets zero and negative flags accordingly
uint8_t nes6502::TYA() {
	a = y;
	SetFlag(Z, a == 0x00);
	SetFlag(N, a & 0x80);
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

// Disassembly function.
// This turs the binary instruction code into something humanly readable for a certain address range
// This is mainly going to be used for debugging
std::map<uint16_t, std::string> nes6502::disassemble(uint16_t nStart, uint16_t nStop) {
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convinient utility to convert variables into hex strings
	auto hex = [](uint32_t n, uint8_t d) {
		std::string s(d, '0');
		// n >>= 4 means n = right shift 4 (n = n >> 4)
		for (int i = d - 1; i >= 0; i--, n >>= 4) {
			s[i] = "0123456789ABCDEF"[n & 0xF]; // n & 0xF is equivalent to n % 16
		}
		return s;
	};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the lookup table
	// as to how many additional bytes we need to read and what the 
	// addressing mode is. This info is used to assemble human readable
	// syntax, which is different depending on the mode.

	// As the instruction is decoded, a std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop) {
		line_addr = addr;

		// Prefix line with instruction address
		std::string s_inst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = bus->cpuRead(addr, true);
		addr++;
		s_inst += lookup[opcode].name + " ";

		// Get operands from desired locations, and form the 
		// instruction based on its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the instruction.
		if (lookup[opcode].addrmode == &nes6502::IMP) {
			s_inst += " {IMP}";
		}
		else if (lookup[opcode].addrmode == &nes6502::IMM) {
			// The data will be in the next byte of the program
			value = bus->cpuRead(addr, true);
			addr++;
			// Including the next byte of the program
			s_inst += "$" + hex(value, 2) + " {IMM}";
		}
		else if (lookup[opcode].addrmode == &nes6502::ZP0) {
			// Zero page addressing sets the page (first byte of memory addr) to 0
			lo = bus->cpuRead(addr, true);
			addr++;
			hi = 0x00;
			s_inst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (lookup[opcode].addrmode == &nes6502::ZPX) {
			// Zero page addressing with x reg offset
			lo = bus->cpuRead(addr, true);
			addr++;
			hi = 0x00;
			s_inst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (lookup[opcode].addrmode == &nes6502::ZPY) {
			// Zero page addressing with y reg offset
			lo = bus->cpuRead(addr, true);
			addr++;
			hi = 0x00;
			s_inst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (lookup[opcode].addrmode == &nes6502::ABS) {
			lo = bus->cpuRead(addr, true);
			addr++;
			hi = bus->cpuRead(addr, true);
			addr++;
			s_inst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (lookup[opcode].addrmode == &nes6502::ABX) {
			lo = bus->cpuRead(addr, true);
			addr++;
			hi = bus->cpuRead(addr, true);
			addr++;
			s_inst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (lookup[opcode].addrmode == &nes6502::ABY) {
			lo = bus->cpuRead(addr, true);
			addr++;
			hi = bus->cpuRead(addr, true);
			addr++;
			s_inst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (lookup[opcode].addrmode == &nes6502::IND) {
			// The supplied 16-bit addr is read to get the actual 16-bit addr
			lo = bus->cpuRead(addr, true);
			addr++;
			hi = bus->cpuRead(addr, true);
			addr++;
			// the () mean a pointer
			s_inst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (lookup[opcode].addrmode == &nes6502::IZX) {
			//Supplied 8-bit address is offset by the X register
			// to a location in page 0x00
			lo = bus->cpuRead(addr, true);
			addr++;
			hi = 0x00;
			s_inst += "$" + hex(lo, 2) + ", X {IZX}";
		}
		else if (lookup[opcode].addrmode == &nes6502::IZY) {
			//Supplied 8-bit address is offset by the Y register
			// to a location in page 0x00
			lo = bus->cpuRead(addr, true);
			addr++;
			hi = 0x00;
			s_inst += "$" + hex(lo, 2) + ", Y {IZY}";
		}
		else if (lookup[opcode].addrmode == &nes6502::REL) {
			value = bus->cpuRead(addr, true);
			addr++;
			s_inst += "$" + hex(value, 2) + " [$" + hex((int8_t)value + addr, 4) + "] {REL}";
		}

		// Add the formed string to an std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so a simple
		// incremental index is not sufficient.
		mapLines[line_addr] = s_inst;
	}

	return mapLines;
}