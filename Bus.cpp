#include "Bus.h"

Bus::Bus()
{
	// Clear RAM contents, just in case
	for (auto& i : ram) i = 0x0;

	// Connect CPU to communication bus
	cpu.ConnectBus(this);
}

Bus::~Bus()
{
}


void Bus::write(uint16_t addr, uint8_t data) 
{
	// Check for correct address locations
	if (addr >= 0x0000 && addr <= 0xFFFF)
		// Overwrite the byte at addr with data
		ram[addr] = data;
}


uint8_t Bus::read(uint16_t addr, bool bReadOnly = false) 
{
	// Check for correct address locations
	if (addr >= 0x0000 && addr <= 0xFFFF)
		// Return byte at given addr
		return ram[addr];

	return 0x00;
}