#include "Bus.h"

Bus::Bus()
{
	// Clear RAM contents upon construction, just in case
	// for (auto& i : cpuRam) i = 0x0;

	// Connect CPU to communication bus
	cpu.ConnectBus(this);
}

Bus::~Bus()
{
}


void Bus::cpuWrite(uint16_t addr, uint8_t data) 
{
	// The cartridge gets priority over what happens on the bus
	// But for now this should be irrelevant and possibly even a bug source
	if (cart->cpuWrite(addr, data)) {

	}
	// Check for correct address locations
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		// Uses mirroring with 2kb
		cpuRam[addr & 0x07FF] = data;
	}
	else if (addr >= 2000 && addr <= 0x3FFF) {
		// Uses mirroring and writes directly to the PPU
		ppu.cpuWrite(addr & 0x0007, data);
	}
}


uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly) 
{
	uint8_t data = 0x00;

	// If the cartridge is trying to interfere, nothing happens
	if (cart->cpuRead(addr, data)) {

	}
	// Check for correct address locations (8kb range for RAM)
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		data = cpuRam[addr & 0x07FF];
	}
	else if (addr >= 2000 && addr <= 0x3FFF) {
		ppu.cpuRead(addr & 0x0007, bReadOnly);
	}

	// ERASE WHEN DONE TESTING CPU
	// data = cpuRam[addr];

	return data;
}

void Bus::insertCartidge(const std::shared_ptr<Cartridge>& cartridge) {
	this->cart = cartridge;
	ppu.ConnectCartridge(cartridge);
}

void Bus::reset() {
	cpu.reset();
	nSystemClockCounter = 0;
}

void Bus::clock() {

}