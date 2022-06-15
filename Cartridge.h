#pragma once
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>

// #include "Bus.h"
#include "Mapper_000.h"

class Cartridge
{
public:
	// Includes string to ROM file
	Cartridge(const std::string& sFileName);
	~Cartridge();

private:
	std::vector<uint8_t> vPRGMemory;
	std::vector<uint8_t> vCHRMemory;

	uint8_t nMapperID = 0;
	uint8_t nPRGBanks = 0;
	uint8_t nCHRBanks = 0;

	std::shared_ptr<Mapper> pMapper;

public:
	// These return a boolean in order to tell the calling system 
	// wether the cartridge is handling that read or write
	bool cpuRead(uint16_t addr, uint8_t &data);
	bool cpuWrite(uint16_t addr, uint8_t data);

	bool ppuRead(uint16_t addr, uint8_t &data);
	bool ppuWrite(uint16_t addr, uint8_t data);
};

