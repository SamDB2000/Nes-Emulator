#pragma once
#include <cstdint>

// This is an abstract base class to be the template for all specific
// mappers that show up in different cartridges
class Mapper
{
public:
	Mapper(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper();

public:
	// Takes the address from ppu/cpu and maps them to address for the ROM
	virtual bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
	virtual bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;
	virtual bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
	virtual bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;

protected:
	uint8_t nPRGBanks = 0;
	uint8_t nCHRBanks = 0;
};

