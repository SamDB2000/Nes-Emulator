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

    if (rdonly) {
        // Reading from the PPU registers can affect their contents
        // so this read only option is used for examining the state
        // of the PPU without changing its state. This is only 
        // used for debug
        switch (addr)
        {
        case 0x0000: // Control
            data = control.reg;
            break;
        case 0x0001: // Mask
            data = mask.reg;
            break;
        case 0x0002: // Status
            data = status.reg;
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
    else {
        // The actual ppu registers that react to being read from.
        // Some registers cannot be read from and will return 0x00
        switch (addr) {
        case 0x0000: // Control - not readable
            break;
        case 0x0001: // Mask - not readable
            break;
        case 0x0002: // Status

            // Get only the first three bits
            data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);

            // vblank and latch are cleared after read
            status.vblank = 0;
            address_latch = 0;
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            data = pOAM[oam_addr];
            break;
        case 0x0005: // Scroll - Not Readable
            break;
        case 0x0006: // PPU Address - Not Readable
            break;
        case 0x0007: // PPU Data
            // Reads from the NameTable ram get delayed one cycle,
            // so output buffer which contains the data from the previous
            // read request
            data = ppu_data_buffer;
            // Then update the buffer for next time
            ppu_data_buffer = ppuRead(vram_addr.reg);
            // Howver, if the address was in palette range, the
            // data is not delayed, so it returns immediately
            if (vram_addr.reg >= 0x3F00) data = ppu_data_buffer;
            // All reads from PPU data automatically increment the NT
            // address depending on the mode set in control register.
            // If set to vertical, incrememnt 32 so it skips to the next row
            // othweise it moves horizontally one column
            vram_addr.reg += (control.increment_mode ? 32 : 1);
            break;
        }
    }
    return data;
}

void nes2C02::cpuWrite(uint16_t addr, uint8_t data)
{
    switch (addr) {
    case 0x0000: // Control
        control.reg = data;
        tram_addr.nametable_x = control.nametable_x;
        tram_addr.nametable_y = control.nametable_y;
        break;
    case 0x0001: // Mask
        mask.reg = data;
        break;
    case 0x0002: // Status
        break;
    case 0x0003: // OAM Address
        oam_addr = data;
        break;
    case 0x0004: // OAM Data
        pOAM[oam_addr] = data;
        break;
    case 0x0005: // Scroll
        // First write to scroll register contains X offset in pixel space
        // which we split into coarse and fine x values
        if (address_latch == 0) {
            fine_x = data & 0x07;
            tram_addr.coarse_x = data >> 3;
            address_latch = 1;
        }
        else {
            tram_addr.fine_y = data & 0x07;
            tram_addr.coarse_y = data >> 3;
            address_latch = 0;
        }
        break;
    case 0x0006: // PPU Address
        if (address_latch == 0) {
            // PPU address bus can be accessed by CPU via the ADDR and DATA
            // registers. The first write to this register latches the high byte
            // of the address, and the second is the low byte.
            // Note the writes are stored in the tram register.
            tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
            address_latch = 1;
        }
        else {
            // When a whole address has been written, the internal vram address
            // buffer is updaed. Writing to the PPU is unwise during rendering
            // as the PPU will maintain the vram address automatically whilst
            // rendering the scanline position
            tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
            vram_addr = tram_addr;
            address_latch = 0;
        }
        break;
    case 0x0007: // PPU Data
        ppuWrite(vram_addr.reg, data);

        // Increment mode decides wether we are writing to the nametable
        // Across the x (+1) or the y (+32) axis.
        vram_addr.reg += (control.increment_mode ? 32 : 1);
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
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        // We're in pattern table memory

        // Mask the most significant bit for left or right pattern table
        // >> 12 so that the most significant hex digit in hex is brought down (value = 0 or 1)
        data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
    }
    else if (addr >= 0x2000 && addr <= 0x3EFF) {

        addr &= 0x0FFF;

        // We're in nametable memory
        // NOTE: 0x3000-0x3EFF are mirrors of 0x2000-0x2EFF
        // From wiki on nametables:
        /*(0, 0)     (256, 0)     (511, 0)
            +-----------+-----------+
            |           |           |
            |           |           |
            |   $2000   |   $2400   |
            |           |           |
            |           |           |
    (0, 240)+-----------+-----------+(511, 240)
            |           |           |
            |           |           |
            |   $2800   |   $2C00   |
            |           |           |
            |           |           |
            +-----------+-----------+
        (0, 479)   (256, 479)   (511, 479)
        */
        // For vertical mirroring, the $2000=$2800 and $2400=$2C00
        if (cart->mirror == Cartridge::MIRROR::VERTICAL) {
            if (addr >= 0x0000 && addr <= 0x03FF)
                data = tblName[0][addr & 0x03FF];
            else if (addr >= 0x0400 && addr <= 0x07FF)
                data = tblName[1][addr & 0x03FF];
            else if (addr >= 0x0800 && addr <= 0x0BFF)
                data = tblName[0][addr & 0x03FF];
            else if (addr >= 0x0C00 && addr <= 0x0FFF)
                data = tblName[1][addr & 0x03FF];
        }
        // For vertical mirroring, the $2000=$2400 and $2800=$2C00
        else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL) {
            if (addr >= 0x0000 && addr <= 0x03FF)
                data = tblName[0][addr & 0x03FF];
            else if (addr >= 0x0400 && addr <= 0x07FF)
                data = tblName[0][addr & 0x03FF];
            else if (addr >= 0x0800 && addr <= 0x0BFF)
                data = tblName[1][addr & 0x03FF];
            else if (addr >= 0x0C00 && addr <= 0x0FFF)
                data = tblName[1][addr & 0x03FF];
        }
        // Small note: source code does not use "else if"
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
        data = tblPalette[addr] & (mask.greyscale ? 0x30 : 0x3F);
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
        addr &= 0x0FFF;

        if (cart->mirror == Cartridge::MIRROR::VERTICAL) {
            if (addr >= 0x0000 && addr <= 0x03FF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0400 && addr <= 0x07FF)
                tblName[1][addr & 0x03FF] = data;
            if (addr >= 0x0800 && addr <= 0x0BFF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                tblName[1][addr & 0x03FF] = data;

        }
        else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL) {
            if (addr >= 0x0000 && addr <= 0x03FF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0400 && addr <= 0x07FF)
                tblName[0][addr & 0x03FF] = data;
            if (addr >= 0x0800 && addr <= 0x0BFF)
                tblName[1][addr & 0x03FF] = data;
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                tblName[1][addr & 0x03FF] = data;
        }
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

void nes2C02::reset() {
    fine_x = 0x00;
    address_latch = 0x00;
    ppu_data_buffer = 0x00;
    scanline = 0;
    cycle = 0;
    status.reg = 0x00;
    mask.reg = 0x00;
    vram_addr.reg = 0x0000;
    tram_addr.reg = 0x0000;
    bg_next_tile_id = 0x00;
    bg_next_tile_attr = 0x00;
    bg_next_tile_lsb = 0x00;
    bg_next_tile_msb = 0x00;
    bg_shifter_pattern_lo = 0x0000;
    bg_shifter_pattern_hi = 0x0000;
    bg_shifter_attrib_lo = 0x0000;
    bg_shifter_attrib_hi = 0x0000;
}

// As we go through scanlines and cycles, the PPU is effectively
// a state machine going through the motions of fetching background
// information and sprite information, compositing them into a single
// pixel to be output.
void nes2C02::clock() {
    // Lamda Helper functions:
    auto IncrementScrollX = [&]() {
        // Note: This is not pixel perfect scrolling. That is handled by the
        // data shifters. Here we are operating in the spatial domain of
        // 8x8 tiles.

        // Check if rendering is enabled
        if (mask.background || mask.sprites) {

            // A single name table is 32x30 tiles. As we increment horizontally
            // we may cross into a neighbouring nametable, or wrap around
            // to a neighboring nametable
            if (vram_addr.coarse_x == 31) {

                // Leaving nametable so wrap address around
                vram_addr.coarse_x = 0;
                // Flip target nametable bit
                vram_addr.nametable_x = ~vram_addr.nametable_x;
            }
            else {
                // Staying in current nametable so just increment
                vram_addr.coarse_x++;
            }
        }
    };

    auto IncrementScrollY = [&]() {
        // Check if rendering is enabled
        if (mask.background || mask.sprites) {

            // If possible, just increment the fine y offset
            if (vram_addr.fine_y < 7) {
                vram_addr.fine_y++;
            }
            else {
                // If we have gone beyond beyond the height of a row, we need to
                // increment the row, potentially wrapping into neighboring 
                // vertical nametables. Although, the bottom two rows
                // do not contain tile information. The coarse y offset is used
                // to identify which row of the nametable we want, and the fine
                // y offset is the specific "scanline"
                vram_addr.fine_y = 0;
                
                // Check if we need to swap vertical nametable targets
                if (vram_addr.coarse_y == 29) {
                    // Reset course y offset
                    vram_addr.coarse_y = 0;
                    // And flip the target nametable bit
                    vram_addr.nametable_y = ~vram_addr.nametable_y;
                }
                else if (vram_addr.coarse_y == 31) {
                    // In case the pointer is in the attribute memory,
                    // we juse wrap around the current nametable
                    vram_addr.coarse_y = 0;
                }
                else {
                    // None of the above boundary/wrapping conditions apply
                    vram_addr.coarse_y++;
                }
            }
        }
    };

    auto TransferAddressX = [&]() {
        if (mask.background || mask.sprites) {
            // Transfer from vram to tram
            vram_addr.nametable_x = tram_addr.nametable_x;
            vram_addr.coarse_x = tram_addr.coarse_x;
        }
    };

    auto TransferAddressY = [&]() {
        if (mask.background || mask.sprites) {
            // Transfer from vram to tram
            vram_addr.fine_y = tram_addr.fine_y;
            vram_addr.nametable_y = tram_addr.nametable_y;
            vram_addr.coarse_y = tram_addr.coarse_y;
        }
    };

    // Prepares shifters for rendering
    auto LoadBackgroundShifters = [&]() {
        // Load the whole 8-bit word into the bottom of the shifters
        bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
        bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;
        // For palettes, take the specified bit (0b01...) and inflate it to a full 8 bits
        bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attr & 0b01) ? 0xFF : 0x00);
        bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attr & 0b10) ? 0xFF : 0x00);
    };

    auto UpdateShifters = [&]() {
        if (mask.background) {
            // Just shift by 1
            bg_shifter_pattern_lo <<= 1;
            bg_shifter_pattern_hi <<= 1;
            bg_shifter_attrib_lo <<= 1;
            bg_shifter_attrib_hi <<= 1;
        }

        // Foreground shifters, only on visible cycles
        if (mask.sprites && cycle >= 1 && cycle < 258) {
            for (int i = 0; i < sprite_count; i++) {
                // Decrement it's x position until we've hit the sprite
                if (spriteScanline[i].x > 0) {
                    spriteScanline[i].x--;
                }
                // Left-shift the shifters so that the bits can be rendered
                else {
                    sprite_shifter_pattern_lo[i] <<= 1;
                    sprite_shifter_pattern_hi[i] <<= 1;
                }
            }
        }
    };

    // https://www.nesdev.org/wiki/PPU_rendering
    // This is where I'm getting all the information on rendering that I'm performing here
    // 
    // Pre-render line -1 (or 261 depending on implementation)
    // This is a dummy scanline, whose sole purpose is to fill the shift registers with 
    // the data for the first two tiles of the next scanline.
    if (scanline >= -1 && scanline < 240) {
        if (scanline == 0 && cycle == 0) {
            // "Odd Frame" cycle skip
            cycle = 1;
        }
        if (scanline == -1 && cycle == 1) {
            status.vblank = 0;

            // Clear out utility variables for sprites
            status.sprite_overflow = 0;
            status.sprite0_hit = 0;

            for (uint8_t i = 0; i < 8; i++) {
                sprite_shifter_pattern_lo[i] = 0;
                sprite_shifter_pattern_hi[i] = 0;
            }
        }

        if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338)) {
            
            UpdateShifters();

            // In these cycles we are collecting and working with visible data.
            // The "shifters" have been preloaded by the end of the previous
            // scanline with the data for the start of this scanline. Once
            // we leave the visible region, we go dormant until the shifters
            // are preloaded for the next scanline.

            // Extract Nametable Byte, Attribute byte, low bg tile byte, high bg tile byte
            // Then we'll want to incrememnt the loopy register in the y direction
            switch ((cycle - 1) % 8) {
            case 0:
                // When the internal cycle counter loops around eight pixels
                LoadBackgroundShifters();
                // Extract next background tile ID
                // "(vram_addr.reg & 0x0FFF)" : Mask for the relevant 12 bits
                // "| 0x2000" : Offset into nametable space on PPU address bus
                bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));

                // Explanation:
                // The bottom 12 bits of the loopy register provide an index into
                // the 4 nametables, regardless of nametable mirroring configuration.
                // nametable_y(1) nametable_x(1) coarse_y(5) coarse_x(5)
                break;
            case 2:
                // Recall that each nametable has two rows of cells that are not tile
                // information, instead they represent the attribute information that
                // indicates which palettes are applied to which area on the screen.
                // Importantly (and frustratingly) there is not a 1 to 1 correspondence
                // between background tile and palette. Two rows of tile data holds 
                // 64 attributes. Therefore we can assume that the attributes affect
                // 8x8 zones on the screen for that nametable. Given a working resolution
                // of 256x240, we can further assume that each zone is 32x32 pixels
                // in screen space, or 4x4 tiles. Four system palettes are allocated
                // to background rendering, so a palette can be specified using just
                // 2 bits. The attribute bute therefore can specify 4 distinct palettes.
                // Therefore, we can even further assume that a single palette is
                // applied to a 2x2 tile combination of the 4x4 tile zone. The very fact
                // that background tiles "share" a palette locally is the reason why
                // in some games you see distortion in the colors at screen edges.
                // _____________________________________________________________________
                // From my understanding, this is all of the reasoning for how, in
                // the end: each 2 bits of attribute memory correspond to the palettes 
                // of some 2x2 tile zone, while the whole attribute byte covers that 4x4.
                // ---------------------------------------------------------------------
                // 
                // As before when choosing the tile ID, we can use the bottom 12 bits of
                // the loopy register, but we need to make the implementation "coarser"
                // because instead of a specific tile, we want the attribute byte for a
                // group of 4x4 tiles, or in other words, we divide our 32x32 address
                // by 4 to give us an equivalent 8x8 address, and we offset this
                // address into the attribute section of the target nametable.
                // 
                // Reconstruct the 12 bit loopy address into an offset into the
                // attribute memory
                // 
                // "vram_addr.coarse_x >> 2" : integer divide coarse x by 4, 5->3 bits
                // "(vram_addr.coarse_y >> 2) << 3" : integer divide coarse x by 4, 
                //                           5->3 bits, shift to make room for coarse x
                // 
                // Result so far: YX00 00yy yxxx
                // All attribute memory begins at 0x03C0 within a nametable, so OR with
                // result to select target nametable, and attribute byte offset. Finally
                // OR with 0x2000 to offset into nametable address space on PPU bus.
                bg_next_tile_attr = ppuRead(0x23C0 | (vram_addr.nametable_y << 11)
                    | (vram_addr.nametable_x << 10)
                    | ((vram_addr.coarse_y >> 2) << 3)
                    | (vram_addr.coarse_x >> 2));

                // Now we've read the correct attribute byte for a specified address,
                // but the byte itself is broken down futher into the 2x2 tile groups
                // in the 4x4 attribute zone.
                // 
                // The attribute byte is assembled thus: BR(76) BL(54) TR(32) TL(10)
                // +----+----+			    +----+----+
				// | TL | TR |			    | ID | ID |
				// +----+----+ where TL =   +----+----+
				// | BL | BR |			    | ID | ID |
				// +----+----+			    +----+----+
                // 
                // Since we know we can access a tile directly from the 12 bit address, we
                // can analyze the bottom bits of the coarse coordinates to privide us with
                // the correct offset into the 8-bit word, to yield the 2 bits we are
                // actually interested in which specifies the palette for the 2x2 group of
                // tiles. We know if "coarse y % 4 < 2" we are in the top half else bottom half.
                // Likewise, if "coarse x % 4 < 2" we are in the left half else right half.
                // Ultimately we want the bottom two bits of our attribute word to be the
                // palette selected. So shift as required...
                if (vram_addr.coarse_y & 0x02) bg_next_tile_attr >>= 4;
                if (vram_addr.coarse_x & 0x02) bg_next_tile_attr >>= 2;
                bg_next_tile_attr &= 0x03; // grab bottom 2 bits after shifting
                break;
            case 4:
                // Fetch the next background tile LSB bit plane from the pattern memory.
                // The Tile ID has been read from the nametable. We will use this id to
                // index into the pattern memory to find the correct sprite (assuming
                // the sprites lie on 8x8 pixel boundaries in that memory, which they do
                // even though 8x16 sprites exist, as bg tiles are always 8x8).
                //
                // Since the sprites are effectively 1 bit deep, but 8 pixels wide, we
                // can represent a whole sprite row as a single byte, so offsetting 
                // into the pattern memory is easy. In total there is 8kb so we need
                // a 13 bit address
                // 
                // "(control.pattern_background << 12)" : the pattern memory selector 
                //                         from the control register, either 0 or 4k offset
                // "((uint16_t)bg_next_tile_id << 4)"   : the tile id multiplied by 16, as
                //                                        2 lots of 8 rows of 8 pixels
                // "(vram_addr.fine_y)" : Offset into which row based on vscroll offset
                // "+ 0"                : Mental clarity for plane offset
                // Note: No PPU address bus offset required as it starts at 0x0000
                bg_next_tile_lsb = ppuRead((control.bg_tile << 12)
                    + ((uint16_t)bg_next_tile_id << 4)
                    + (vram_addr.fine_y) + 0);
                break; 
            case 6:
                // Fetch the next bg tile MSB bit plane from the pattern memory
                // Same as above, except for a +8 offset to select the next bit plane
                bg_next_tile_msb = ppuRead((control.bg_tile << 12)
                    + ((uint16_t)bg_next_tile_id << 4)
                    + (vram_addr.fine_y) + 8);
                break;
            case 7:
                // Increment the bg tile "pointer" to the next tile horizontally
                // in the nametable memory. Note this may cross nametable boundaries which
                // is a litte complex, but essential to implement scrolling
                IncrementScrollX();
                break;
            }
        }

        if (cycle == 256) {
            IncrementScrollY();
        }

        if (cycle == 257) {
            LoadBackgroundShifters();
            TransferAddressX();
        }

        if (scanline == -1 && cycle >= 280 && cycle < 305) {
            // End of vblank period so reset the Y address ready for rendering
            TransferAddressY();
        }
        
        // Superfluous reads of tile id at end of scanline
        if (cycle == 388 || cycle == 340) {
            bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
        }

        // ======== Foreground Rendering ======================================
        if (cycle == 257 && scanline >= 0) { // Note: this isn't actually cycle accurate
            // Fill spriteScanline array with FF
            // This is a better way than 0's since FF will never be visible on screen
            std::memset(spriteScanline, 0xFF, 8 * sizeof(sObjectAttributeEntry));
            sprite_count = 0;

            // Clear out any residual information in the sprite pattern shifters
            for (uint8_t i = 0; i < 8; i++) {
                sprite_shifter_pattern_lo[i] = 0;
                sprite_shifter_pattern_hi[i] = 0;
            }

            // Clear this flag in case it got set last time
            spriteZeroHitPossible = false;

            uint8_t nOAMEntry = 0;
            // loop through all 64 OAM entries or until 9 sprites are found
            while ((nOAMEntry < 64) && (sprite_count < 9)) {
                // Test if a sprite is visible by subtractig the sprite's y
                // coordindate from the current scanline
                int16_t diff = ((int16_t)scanline - (int16_t)OAM[nOAMEntry].y);
                if (diff >= 0 && diff < (control.sprite_height ? 16 : 8)) {
                    // Sprite is visible on the next scanline
                    // Add to scanline buffer if not already full
                    if (sprite_count <= 8) {
                        // Check for if a sprite zero hit is possible on next scanline
                        if (nOAMEntry == 0) {
                            spriteZeroHitPossible = true;
                        }

                        memcpy(&spriteScanline[sprite_count], &OAM[nOAMEntry], sizeof(sObjectAttributeEntry));
                        sprite_count++;
                    }

                    
                }
                nOAMEntry++;
            }
            status.sprite_overflow = (sprite_count > 8);
        } // End of sprite evaluation phase

        // Now we need to find relevant data from pattern memory
        if (cycle == 340) {
            // Populate shift registers with found sprites
            for (uint8_t i = 0; i < sprite_count; i++) {
                // Local variables to store sprite data and addresses
                uint8_t sprite_pattern_bits_lo = 0x00;
                uint8_t sprite_pattern_bits_hi = 0x00;
                uint16_t sprite_pattern_addr_lo = 0x0000;
                uint16_t sprite_pattern_addr_hi = 0x0000;

                // This is how we find the address of the correct row of pixels 
                // for the sprites we found from OAM
                if (!control.sprite_height) {
                    // 8x8 Sprite Mode
                    if (!(spriteScanline[i].attribute & 0x80)) {
                        // Sprite is NOT flipped vertically (it's normal)
                        // So we can just read directly from the pattern table
                        
                        // We're going to work with just the lo address,
                        // cause the hi addr is just 8 bits higher
                        
                        // Use the bit in ctrl register to shift b/w
                        // pattern tables. Shift left 12 for the 4Kb
                        // size of the pattern table (i.e. $0000 or $1000).
                        // 
                        // Then add the ID shifted by 4 (16 bytes) since
                        // each tile is 16 bytes.
                        // 
                        // Then add the sprite's Y coordinate subtracted
                        // from the current scanline for row offset.
                        sprite_pattern_addr_lo =
                            (control.sprite_pattern << 12)
                            | (spriteScanline[i].id << 4)
                            | (scanline - spriteScanline[i].y);
                        // This gave us the address of a byte of a sprite
                        // in pattern memory.
                    }
                    else {
                        // Sprite is upside down
                        // This is the same process as normal except
                        // the row is subtracted from 7.
                        sprite_pattern_addr_lo =
                            (control.sprite_pattern << 12)
                            | (spriteScanline[i].id << 4)
                            | (7 - (scanline - spriteScanline[i].y));
                    }
                }
                else {
                    // 8x16 Sprite Mode
                    // Note: in 8x16, the ctrl bit for pattern table is ignored
                    if (!(spriteScanline[i].attribute & 0x80)) {
                        // Sprite is NOT flipped vertically (it's normal)
                        if (scanline - spriteScanline[i].y < 8) {
                            // We're in the top half of the tile

                            // ((spriteScanline[i].id & 0x01) << 12)
                            // Gives which pattern table
                            // | ((spriteScanline[i].id & 0xFE) << 4)
                            // Which cell? Tile ID * 16 (16 bytes per tile)
                            sprite_pattern_addr_lo =
                                ((spriteScanline[i].id & 0x01) << 12)
                                | ((spriteScanline[i].id & 0xFE) << 4)
                                | ((scanline - spriteScanline[i].y) & 0x07);
                        }
                        else {
                            // We're reading the bottom half of the tile
                            // The added 1 shifts us down a row
                            sprite_pattern_addr_lo =
                                  ( (spriteScanline[i].id & 0x01)      << 12)
                                | (((spriteScanline[i].id & 0xFE) + 1) << 4 )
                                | ((scanline - spriteScanline[i].y) & 0x07  );
                        }
                    }
                    else {
                        // Sprite is upside down
                        if (scanline - spriteScanline[i].y < 8) {
                            sprite_pattern_addr_lo =
                                ((spriteScanline[i].id & 0x01) << 12)
                                | ((spriteScanline[i].id & 0xFE) << 4)
                                | (7 - (scanline - spriteScanline[i].y) & 0x07);
                        }
                        else {
                            sprite_pattern_addr_lo =
                                ((spriteScanline[i].id & 0x01) << 12)
                                | (((spriteScanline[i].id & 0xFE) + 1) << 4)
                                | (7 - (scanline - spriteScanline[i].y) & 0x07);
                        }
                    }
                }

                // The high bit-plane is always just 8 bytes after the low bit plane
                sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;

                // Now we can read the bits from the addresses we've obtained
                sprite_pattern_bits_lo = ppuRead(sprite_pattern_addr_lo);
                sprite_pattern_bits_hi = ppuRead(sprite_pattern_addr_hi);

                // Handle if the sprite is flipped horizontally (literally flip the byte)
                // attribute byte tells us if it should be flipped  (i.e. mirrored left<->right)
                if (spriteScanline[i].attribute & 0x40) { 
                    // This little lambda function "flips" a byte
                    // so 0b11100000 becomes 0b00000111. It's very
                    // clever, and stolen completely from here:
                    // https://stackoverflow.com/a/2602885
                    auto flipbyte = [](uint8_t b)
                    {
                        b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
                        b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
                        b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
                        return b;
                    };

                    // Flip patterns horizontally
                    sprite_pattern_bits_lo = flipbyte(sprite_pattern_bits_lo);
                    sprite_pattern_bits_hi = flipbyte(sprite_pattern_bits_hi);
                }

                // Now we can finally load the pattern into our sprite shift
                // registers, ready for rendering on the next scanline
                sprite_shifter_pattern_lo[i] = sprite_pattern_bits_lo;
                sprite_shifter_pattern_hi[i] = sprite_pattern_bits_hi;
            }
        }
    }

    if (scanline == 240) {
        // Post Render Scanline - do nothing
    }

    if (scanline >= 241 && scanline < 261) {
        // The VBlank flag of the PPU is set at tick 1 (the second tick) of scanline 241, 
        // where the VBlank NMI also occurs. 
        // The PPU makes no memory accesses during these scanlines, 
        // so PPU memory can be freely accessed by the program
        if (scanline == 241 && cycle == 1) {
            status.vblank = 1;
            if (control.nmi_enable)
                // The nmi variable is handled by the bus and calls the cpu nmi() function
                nmi = true;
        }
    }

    uint8_t bg_pixel = 0x00; // 2-bit pixel to be rendered
    uint8_t bg_palette = 0x00; // 3-bit index of the palette the pixel indexes

    if (mask.background) {
        // Select which bit of the shift register depending on the fine_x value
        uint16_t bit_mux = 0x8000 >> fine_x;

        // Select plane pixels by extracting from the shifter at the required location
        uint8_t p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
        uint8_t p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;
        bg_pixel = (p1_pixel << 1) | p0_pixel; // Combine

        uint8_t bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
        uint8_t bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;
        bg_palette = (bg_pal1 << 1) | bg_pal0;
    }

    uint8_t fg_pixel = 0x00;
    uint8_t fg_palette = 0x00;
    uint8_t fg_priority = 0x00;
    uint8_t zero_hit = 0x00;

    if (mask.sprites) {
        // Clear sprite zero rendered flag
        spriteZeroHitRendered = false;

        // Find the first visible pixel of the highest priority
        for (uint8_t i = 0; i < sprite_count; i++) {
            // Sprites naturally are in order since we got them from the OAM
            // Array that we iterated through in order
            if (spriteScanline[i].x == 0) {
                // The shifters should give us output to draw
                // Extract the MSB from each shifter
                uint8_t fg_pixel_lo = (sprite_shifter_pattern_lo[i] & 0x80) > 0;
                uint8_t fg_pixel_hi = (sprite_shifter_pattern_hi[i] & 0x80) > 0;
                // Put together
                fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;

                fg_palette = (spriteScanline[i].attribute & 0x03) + 0x04;
                // The priority is the sprite's priority with the background
                // i.e. sprites can actually go behind/in front of the background
                fg_priority = (spriteScanline[i].attribute & 0x20) == 0;

                // If fg_pixel is not transparent, render it and don't bother
                // checking the rest of the sprites on the scanline (priority)
                if (fg_pixel != 0) {
                    // If the bg and fg pixels are not transparent, there could
                    // be a sprite zero hit. If we're on sprite 0
                    if (spriteZeroHitPossible && i == 0) {
                        spriteZeroHitRendered = true;
                    }

                    break;
                }

            }
        }
    }

    uint8_t pixel = 0x00; // The final pixel
    uint8_t palette = 0x00; // The final palette

    if (fg_pixel == 0 && bg_pixel == 0) {
        // Transparent bg and fg - drawn as default color
        // No change to pixel/palette
    }
    else if (bg_pixel == 0 && fg_pixel > 0) {
        // Draw the fg
        pixel = fg_pixel;
        palette = fg_palette;
    }
    else if (fg_pixel == 0 && bg_pixel > 0) {
        // draw the background
        pixel = bg_pixel;
        palette = bg_palette;
        // Note: this is what allows us to have sprites that
        // are not rectangular i.e. partially transparent
    }
    else if (fg_pixel > 0 && bg_pixel > 0) {
        // It's possible to for a sprite zero hit if both the
        // background and foreground are visible
        if (spriteZeroHitRendered) {
            if (mask.background & mask.sprites) {
                if (~(mask.left_bg | mask.left_sprites)) {
                    if (cycle >= 9 && cycle < 258) {
                        status.sprite0_hit = 1;
                    }
                }
                else
                {
                    if (cycle >= 1 && cycle < 258) {
                        status.sprite0_hit = 1;
                    }
                }
            }
        }

        // Both are visible... How do we choose?
        // Use the fg_priority flag that we obtained
        if (fg_priority) {
            // Draw foreground
            pixel = fg_pixel;
            palette = fg_palette;
        }
        else {
            pixel = bg_pixel;
            palette = bg_palette;
        }
    }


    // Display output based on palette and pixel
    sprScreen.SetPixel(cycle - 1, scanline, GetColorFromPaletteRam(palette, pixel));

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

/* Simply returns the current sprite holding the rendered screen*/
olc::Sprite& nes2C02::GetScreen()
{
    return sprScreen;
}

olc::Sprite& nes2C02::GetNameTable(uint8_t i) {
    return sprNameTable[i];
}

// This function draws the CHR ROM for a given pattern table into
// an olc::Sprite, using a specified palette. 
olc::Sprite& nes2C02::GetPatternTable(uint8_t i, uint8_t palette) {
    // Pattern tables consist of 16x16 "tiles or characters". 
    // It is independent of the running emulation and 
    // it does not change the systems state, though it gets
    // all the data it needs from the live system. Consequently, if the
    // game has not yet established palettes or mapped to relevent
    // CHR ROM banks, the sprite may look empty. This approach permits a
    // "live" extraction of the pattern table exactly how the NES, and 
    // ultimately the playet would see it

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
                    uint8_t pixel = ((tile_lsb & 0x01) << 1) | (tile_msb & 0x01);

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