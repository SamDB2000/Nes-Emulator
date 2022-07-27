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
	} header;

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
			// Find the size of program memory (# of PRGbanks)
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

		// Load appropriate mapper
		switch (nMapperID) {
		case 0:
			// Uses polymorphism to have any mapper type fit the pMapper variable
			pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks);
			break;
		}

		ifs.close();
	}
}

Cartridge::~Cartridge() {}

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data)
{
	// Temp variable for transformed address
	uint32_t mapped_addr = 0;
	
	// Only change the mapped address if the specific 
	// mapper determines it must come from the cartridge
	if (pMapper->cpuMapRead(addr, mapped_addr)) {
		// Set data to the memory from the PRG vector at the given mapped offset
		// Remember that vPRGMemory is read directly from the ROM file
		data = vPRGMemory[mapped_addr];
		return true;
	} // If the cartridge isn't read from, don't need to do anything
	else
		return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->cpuMapWrite(addr, mapped_addr)) {
		vPRGMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t& data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->ppuMapRead(addr, mapped_addr)) {
		vCHRMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (pMapper->ppuMapWrite(addr, mapped_addr)) {
		vCHRMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}
