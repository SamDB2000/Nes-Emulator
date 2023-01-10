#pragma once
#include <cstdint>
#include <array>

#include "nes6502.h"
#include "nes2C02.h"
#include "Cartridge.h"

class Bus
{
public:
	Bus();
	~Bus();

public: // Devices on the NES
	// The 6502 CPU
	nes6502 cpu;

	// The 2C02 Picture Processing Unit
	nes2C02 ppu;

	// 2k RAM
	std::array<uint8_t, 2048> cpuRam;
	// uint8_t cpuRam[64 * 1024];

	// The Cartridge
	std::shared_ptr<Cartridge> cart;

	// The controllers (the NES only supported 2)
	// Stores the instantaneous state of both the controllers
	uint8_t controller[2];

public: // Bus Read and Write
	void cpuWrite(uint16_t addr, uint8_t data);
	uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

public: // System Interface
	void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void reset();
	void clock();

private:
	// A count of how many clock ticks have passed
	uint32_t nSystemClockCounter = 0;

	// Store the snapshot of the input when the corresponding
	// memory address is written to
	uint8_t controller_state[2];

	// Direct Memory Access is used to swiftly transfer data from the CPU
	// bus into the OAM memory. The program prepares a page of memory with
	// the sprite info required for the next frame and then initiates a
	// DMA transfer. This suspends the CPU momentarily while the PPU gets
	// sent data at PPU clock speeds. Note here, that dma_page and dma_addr
	// form a 16-bit address in the CPU bus address space
	uint8_t dma_page = 0x00;
	uint8_t dma_addr = 0x00; // represents the low byte of a page
	uint8_t dma_data = 0x00; // represents the byte of data in transit

	// A flag to indicate that a DMA transfer is happening
	bool dma_transfer = false;

	// DMS transfers need to be timed accurately. In principle it takes
	// 512 cycles to read and write the 256 bytes of the OAM memory, a
	// read followed by a write. However, the CPU needs to be on an "even"
	// clock cycle, so a dymmy cycle of idleness may be required
	bool dma_dummy = true;
};

