#pragma once
#include <cstdint>

#include "Cartridge.h"

// I feel weird about using this... remove when able
#include "olcPixelGameEngine.h"



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
	// This normally exists on the cartridge (not part of the PPU hardware)
	// This will be unused for base emulation but possibly used when making cool stuff with the emulator
	uint8_t tblPattern[2][4096];

	

public:
	// Communications from the PPU to the cpu bus
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

private:
	// Array for all the colors the NES was cable of creating
	olc::Pixel palScreen[0x40];
	// This will represent the full screen output
	olc::Sprite sprScreen = olc::Sprite(256, 240);
	// This sprite will be a graphical representation of both nametables
	olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256, 240) };
	// This sprite will represent both patterntables
	olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) };

public:
	// Debugging Utilities
	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t i);
	olc::Sprite& GetPatternTable(uint8_t i);
	bool frame_complete = false;

private:
	int16_t scanline = 0;
	int16_t cycle = 0;
};

