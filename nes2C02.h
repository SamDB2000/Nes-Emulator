#pragma once
#include <cstdint>
#include "Bus.h"

class nes2C02
{
public:
	nes2C02();
	~nes2C02();

private:
	// VRAM to hold name table information (2kb's or two whole name tables of 1kb size)
	uint8_t tblName[2][1024];
	// VRAM for palette table (32 entries)
	uint8_t tblPalette[32];
	// Two 4kb arrays that represent pattern memory
	uint8_t tblPattern[2][4096]; // Javid future reminder (see if this is unnecessary later)

public:
	// Communications with main bus
	uint8_t	cpuRead(uint16_t addr, bool rdonly = false);
	void cpuWrite(uint16_t addr, uint8_t data);

	// Communications with PPU bus
	uint8_t ppuRead(uint16_t addr, bool rdonly = false);
	void ppuWrite(uint16_t addr, uint8_t data);

private:
	// The Cartridge Object
	std::shared_ptr<Cartridge> cart;

public:
	// Interface
	void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void clock();
};

