/*** This is the PPU or Picture Processing Unit */
#include "nes2C02.h"

nes2C02::nes2C02() {

    // Fill the entry with all 64 pixel colors that the PPU was capable of displaying
    palScreen[0x00] = olc::Pixel(84, 84, 84);
    palScreen[0x01] = olc::Pixel(0, 30, 116);
    palScreen[0x02] = olc::Pixel(8, 16, 144);
    palScreen[0x03] = olc::Pixel(48, 0, 136);
    palScreen[0x04] = olc::Pixel(68, 0, 100);
    palScreen[0x05] = olc::Pixel(92, 0, 48);
    palScreen[0x06] = olc::Pixel(84, 4, 0);
    palScreen[0x07] = olc::Pixel(60, 24, 0);
    palScreen[0x08] = olc::Pixel(32, 42, 0);
    palScreen[0x09] = olc::Pixel(8, 58, 0);
    palScreen[0x0A] = olc::Pixel(0, 64, 0);
    palScreen[0x0B] = olc::Pixel(0, 60, 0);
    palScreen[0x0C] = olc::Pixel(0, 50, 60);
    palScreen[0x0D] = olc::Pixel(0, 0, 0);
    palScreen[0x0E] = olc::Pixel(0, 0, 0);
    palScreen[0x0F] = olc::Pixel(0, 0, 0);

    palScreen[0x10] = olc::Pixel(152, 150, 152);
    palScreen[0x11] = olc::Pixel(8, 76, 196);
    palScreen[0x12] = olc::Pixel(48, 50, 236);
    palScreen[0x13] = olc::Pixel(92, 30, 228);
    palScreen[0x14] = olc::Pixel(136, 20, 176);
    palScreen[0x15] = olc::Pixel(160, 20, 100);
    palScreen[0x16] = olc::Pixel(152, 34, 32);
    palScreen[0x17] = olc::Pixel(120, 60, 0);
    palScreen[0x18] = olc::Pixel(84, 90, 0);
    palScreen[0x19] = olc::Pixel(40, 114, 0);
    palScreen[0x1A] = olc::Pixel(8, 124, 0);
    palScreen[0x1B] = olc::Pixel(0, 118, 40);
    palScreen[0x1C] = olc::Pixel(0, 102, 120);
    palScreen[0x1D] = olc::Pixel(0, 0, 0);
    palScreen[0x1E] = olc::Pixel(0, 0, 0);
    palScreen[0x1F] = olc::Pixel(0, 0, 0);

    palScreen[0x20] = olc::Pixel(236, 238, 236);
    palScreen[0x21] = olc::Pixel(76, 154, 236);
    palScreen[0x22] = olc::Pixel(120, 124, 236);
    palScreen[0x23] = olc::Pixel(176, 98, 236);
    palScreen[0x24] = olc::Pixel(228, 84, 236);
    palScreen[0x25] = olc::Pixel(236, 88, 180);
    palScreen[0x26] = olc::Pixel(236, 106, 100);
    palScreen[0x27] = olc::Pixel(212, 136, 32);
    palScreen[0x28] = olc::Pixel(160, 170, 0);
    palScreen[0x29] = olc::Pixel(116, 196, 0);
    palScreen[0x2A] = olc::Pixel(76, 208, 32);
    palScreen[0x2B] = olc::Pixel(56, 204, 108);
    palScreen[0x2C] = olc::Pixel(56, 180, 204);
    palScreen[0x2D] = olc::Pixel(60, 60, 60);
    palScreen[0x2E] = olc::Pixel(0, 0, 0);
    palScreen[0x2F] = olc::Pixel(0, 0, 0);

    palScreen[0x30] = olc::Pixel(236, 238, 236);
    palScreen[0x31] = olc::Pixel(168, 204, 236);
    palScreen[0x32] = olc::Pixel(188, 188, 236);
    palScreen[0x33] = olc::Pixel(212, 178, 236);
    palScreen[0x34] = olc::Pixel(236, 174, 236);
    palScreen[0x35] = olc::Pixel(236, 174, 212);
    palScreen[0x36] = olc::Pixel(236, 180, 176);
    palScreen[0x37] = olc::Pixel(228, 196, 144);
    palScreen[0x38] = olc::Pixel(204, 210, 120);
    palScreen[0x39] = olc::Pixel(180, 222, 120);
    palScreen[0x3A] = olc::Pixel(168, 226, 144);
    palScreen[0x3B] = olc::Pixel(152, 226, 180);
    palScreen[0x3C] = olc::Pixel(160, 214, 228);
    palScreen[0x3D] = olc::Pixel(160, 162, 160);
    palScreen[0x3E] = olc::Pixel(0, 0, 0);
    palScreen[0x3F] = olc::Pixel(0, 0, 0);
}

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
        status.vblank = 1; // this is a hack for now
        // Get only the first three bits
        data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);

        // vblank and latch are cleared after read
        status.vblank = 0;
        address_latch = 0;
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
        // set output to what's currently buffered
        data = ppu_data_buffer;
        // Since it takes more than one cycle to do this, we first buffer it
        ppu_data_buffer = ppuRead(ppu_address);

        // Doesn't work for all addresses (doesn't take multiple cyclces for palettes)
        if (ppu_address > 0x3F00) data = ppu_data_buffer;
        // ppu_address++;
        break;
    }

    return data;
}

void nes2C02::cpuWrite(uint16_t addr, uint8_t data)
{
    switch (addr) {
    case 0x0000: // Control
        control.reg = data;
        break;
    case 0x0001: // Mask
        mask.reg = data;
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
        if (address_latch == 0) {
            ppu_address = (ppu_address & 0x00FF) | (data << 8);
            address_latch = 1;
        }
        else {
            ppu_address = (ppu_address & 0xFF00) | data;
            address_latch = 0;
        }
        break;
    case 0x0007: // PPU Data
        ppuWrite(ppu_address, data);
        ppu_address++;
        break;
    }
}

uint8_t nes2C02::ppuRead(uint16_t addr, bool rdonly)
{
    uint8_t data = 0x00;
    // Masking in order to not exceed memory range
    addr &= 0x3FFF; 

    // Option of reading from the cartridge first
   /* if (cart->ppuRead(addr, data)) {

    }*/
    // else 
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        // We're in pattern table memory

        // Mask the most significant bit for left or right pattern table
        // >> 12 so that the most significant hex digit in hex is brought down (value = 0 or 1)
        data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
    }
    else if (addr >= 0x2000 && addr <= 0x3EFF) {
        // We're in nametable memory
        // NOTE: 0x3000-0x3EFF are mirrors of 0x2000-0x2EFF
    }
    else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        // We're in Palette RAM
        // NOTE: 0x3F20-0x3FFF are mirrors of 0x3F00-0x3F1F

        // Get the right index for our tblPalette by masking the bottom 5 bits
        addr &= 0x001F;

        // Addresses $3F10/$3F14/$3F18/$3F1C are mirrors of $3F00/$3F04/$3F08/$3F0C
        // Note that this goes for writing as well as reading
        // Fun fact: The sky will be black in Super Mario Bros. if this is not
        // implemented since it writes the backdrop color through $3F10
        //
        // We're just hardcoding the bg color mirroring
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;
        
        // Use the tblPalette array that we're using as VRAM
        data = tblPalette[addr];
    }

    return data;
}

void nes2C02::ppuWrite(uint16_t addr, uint8_t data)
{
    addr &= 0x3FFF;

    // Option to write to the cartridge first
    if (cart->ppuWrite(addr, data)) {

    }
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        // We're in pattern table memory

        // Pattern memory is usually a ROM so this may not be necessary
        tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
    }
    else if (addr >= 0x2000 && addr <= 0x3EFF) {
        // We're in nametable memory
        // NOTE: 0x3000-0x3EFF are mirrors of 0x2000-0x2EFF
    }
    else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        // We're in Palette RAM
        // NOTE: 0x3F20-0x3FFF are mirrors of 0x3F00-0x3F1F

        // Get the right index for our tblPalette by masking the bottom 5 bits
        addr &= 0x001F;

        // Addresses $3F10/$3F14/$3F18/$3F1C are mirrors of $3F00/$3F04/$3F08/$3F0C
        // Note that this goes for writing as well as reading
        // Fun fact: The sky will be black in Super Mario Bros. if this is not
        // implemented since it writes the backdrop color through $3F10
        //
        // We're just hardcoding the bg color mirroring
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;

        // Use the tblPalette array that we're using as VRAM
        tblPalette[addr] = data;
        
    }
}

void nes2C02::ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge) {
    this->cart = cartridge;
}

void nes2C02::clock() {
    // Fake some noise for now
    sprScreen.SetPixel(cycle - 1, scanline, palScreen[(rand() % 2) ? 0x3F : 0x30]);


    // Advance renderer - it never stops, it's relentless
    cycle++;

    if (cycle >= 341) {
        cycle = 0;
        scanline++;
        if (scanline >= 261) {
            scanline = -1;
            frame_complete = true;
        }
    }
}

olc::Sprite& nes2C02::GetScreen()
{
    return sprScreen;
}

olc::Sprite& nes2C02::GetNameTable(uint8_t i) {
    return sprNameTable[i];
}

olc::Sprite& nes2C02::GetPatternTable(uint8_t i, uint8_t palette) {
    // For a given pattern table there are 16x16 tiles
    for (uint16_t nTileY = 0; nTileY < 16; nTileY++) {
        for (uint16_t nTileX = 0; nTileX < 16; nTileX++) {
            // Want to turn 2d coordinate into a 1D coordinate to index the pattern memory (byte offset)
            // 16 tiles of 16 bytes for Y, 16 bytes per tile for X
            uint16_t nOffset = (nTileY * 16 * 16) + nTileX * 16;

            // for each tile we have 8 rows of 8 pixels
            for (uint16_t row = 0; row < 8; row++) {
     
                // For each row, we need to read both bit planes of the character
                // in order to extreact the least significant and most significant
                // bits of the 2 bit pixel value. Each character is stored as 64 bits
                // of lsb, followed by 64 bits of msb. This conviniently means that
                // two corresponding rows are always 8 bytes apart in memory.

                // Each pattern table is 4kb. i gives which pattern table.
                // We then add the offset and row for the specific byte.
                // We read from the least significant bit plane first.
                uint8_t tile_lsb = ppuRead(i * 0x1000 + nOffset + row + 0x0000);
                uint8_t tile_msb = ppuRead(i * 0x1000 + nOffset + row + 0x0008);

                // These point to two bytes each containing 8 pixels worth of data
                // We need to combine the bytes to give us the final bitmap color (0,1,2,3)
                for (uint16_t col = 0; col < 8; col++) {
                    // Adding the least significant bit of each byte for the pixel color
                    uint8_t pixel = (tile_lsb & 0x01) + (tile_msb & 0x01);
                    // Bit-shift by 1 so next iteration the least significant bits are correct
                    tile_lsb >>= 1; tile_msb >>= 1;

                    // Now we have an NES pixel value and location, so let's draw that into 
                    // the olc sprite that represents this section of the pattern memory. 
                    sprPatternTable[i].SetPixel(
                        nTileX * 8 + (7 - col), // Because we're using the lsb of the row word first
                                                // we are effectively reading the row from right to left,
                                                // so we need to draw the row "backwards"
                        nTileY * 8 + row,
                        GetColorFromPaletteRam(palette, pixel)
                    );
                }
            }
        }
    }
    return sprPatternTable[i];
}

// Uses the 2-bit palette and pixel values to return the correct color of a pixel
olc::Pixel& nes2C02::GetColorFromPaletteRam(uint8_t palette, uint8_t pixel) {
    // pixel value gives two bits corresponding to the color in the palette
    // palette id (2-bits) gives which palette (0-3 = bg, 4-7 = fg)
    // 0x3F00 - offset into memory where palettes are stored
    // & 0x3F - used to make sure we don't overstep palScreen array bounds
    return palScreen[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];

    // Note: we don't access the tblPalette directly, instead we know that ppuRead()
    // will map the address onto the seperate small RAM attached to the PPU bus.
}