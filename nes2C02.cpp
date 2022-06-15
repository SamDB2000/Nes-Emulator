/*** This is the PPU or Picture Processing Unit */
#include "nes2C02.h"

nes2C02::nes2C02() {}

nes2C02::~nes2C02() {}

uint8_t nes2C02::cpuRead(uint16_t addr, bool rdonly)
{
    uint8_t data = 0x00;
    
    // The cpu can only address 8 different functions on the ppu
    // Leaving the functionality out for now
    switch (addr) {
    case 0x0000: // Control
        break;
    case 0x0001: // Mask
        break;
    case 0x0002: // Status
        break;
    case 0x0003: // OAM Address
        break;
    case 0x0004: // OAM Data
        break;
    case 0x0005: // Scroll
        break;
    case 0x0006: // PPU Address
        break;
    case 0x0007: // PPU Data
        break;
    }

    return data;
}

void nes2C02::cpuWrite(uint16_t addr, uint8_t data)
{
    switch (addr) {
    case 0x0000: // Control
        break;
    case 0x0001: // Mask
        break;
    case 0x0002: // Status
        break;
    case 0x0003: // OAM Address
        break;
    case 0x0004: // OAM Data
        break;
    case 0x0005: // Scroll
        break;
    case 0x0006: // PPU Address
        break;
    case 0x0007: // PPU Data
        break;
    }
}

uint8_t nes2C02::ppuRead(uint16_t addr, bool rdonly)
{
    uint8_t data = 0x00;
    // Masking in order to not exceed memory range
    addr &= 0x3FFF; 

    // Option of reading from the cartridge first
    if (cart->ppuRead(addr, data)) {

    }
    return data;
}

void nes2C02::ppuWrite(uint16_t addr, uint8_t data)
{
    addr &= 0x3FFF;

    // Option to write to the cartridge first
    if (cart->ppuWrite(addr, data)) {

    }
}

void nes2C02::ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    this->cart = cartridge;
}

void nes2C02::clock() {
    //cycle++;
}