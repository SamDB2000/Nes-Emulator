#include "Mapper_000.h"

// Implementation specifics taken from https://www.nesdev.org/wiki/NROM

Mapper_000::Mapper_000(uint8_t prgBanks, uint8_t chrBanks) : Mapper(prgBanks, chrBanks)
{
}

Mapper_000::~Mapper_000()
{
}

bool Mapper_000::cpuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    // If the address supplied by the CPU is within the cartridge range
    // Determine the mapped address
    // If there is more than one bank (aka 32kb memory)
    // We make the given address to make it fit in the range 0x0000 to 0x7FFF allowed by the ROM
    // Otherwise we mirror it within the 16kb range
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        // If PRGROM is 32kb
        //      CPU Address Bus         PRG ROM
        //      0x8000 -> 0xFFFF: Map   0x0000 -> 0x7FFF
        // If PRGROM is 16kb
        //      CPU Address Bus         PRG ROM
        //      0x8000 -> 0xBFFF: Map   0x0000 -> 0x3FFF
        //      0xC000 -> 0xFFFF:Mirror 0x0000 -> 0x3FFF
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_000::cpuMapWrite(uint16_t addr, uint32_t& mapped_addr)
{
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        mapped_addr = addr & (nPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }
    return false;
}

bool Mapper_000::ppuMapRead(uint16_t addr, uint32_t& mapped_addr)
{
    // Acceptable range only includes the Pattern Tables
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        // No mapping needed for PPU
        mapped_addr = addr;
        return true;
    }
    return false;
}

bool Mapper_000::ppuMapWrite(uint16_t addr, uint32_t& mapped_addr)
{

    //if (addr >= 0x0000 && addr <= 0x1fff) {
    //    return true;
    //}

    // Just returns false, the PPU doesn't need to write to it
    return false;
}
