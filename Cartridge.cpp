#include "Cartridge.h"

Cartridge::Cartridge(const std::string& sFileName) {

	// iNES Format Header
	struct sHeader
	{
		char name[4];
		uint8_t prg_rom_chunks;
		uint8_t chr_rom_chunks;
		uint8_t mapper1;
		uint8_t mapper2;
		uint8_t prg_ram_size;
		uint8_t tv_system1;
		uint8_t tv_system2;
		char unused[5];
	} header; // we're creating a header variable after declaration
	// Didn't know that till now!

	std::ifstream ifs; // input file stream
	// open file in binary
	ifs.open(sFileName, std::ifstream::binary);
	if (ifs.is_open()) {
		// Read file header
		ifs.read((char*)&header, sizeof(sHeader));

		// next 512 bytes are training information (we ignore it)
		if (header.mapper1 & 0x04) {
			ifs.seekg(512, std::ios_base::cur);
		}

		// Determine Mapper ID
		nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);

		// There are 3 types of iNES files, for now we'll focus on type 1

		uint8_t fileType = 1;

		if (fileType == 0) {

		}

		if (fileType == 1) {
			// Find the size of program memory
			nPRGBanks = header.prg_rom_chunks;
			// Resive vRAM based on # of banks (16kb per bank of prg memory)
			vPRGMemory.resize(nPRGBanks * 16384);
			// Read the data from the file into the vector
			ifs.read((char*)vPRGMemory.data(), vPRGMemory.size());

			// Find the size of chr memory
			nCHRBanks = header.chr_rom_chunks;
			// Resive vRAM based on # of banks (8kb per bank of chr memory)
			vCHRMemory.resize(nCHRBanks * 8192);
			// Read the data from the file into the vector
			ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());


		}

		if (fileType == 2) {

		}

		ifs.close();
	}
}

Cartridge::~Cartridge() {}

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data)
{
	return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
	return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t& data)
{
	return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
	return false;
}
