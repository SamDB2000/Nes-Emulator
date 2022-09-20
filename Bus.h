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
};

