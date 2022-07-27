#pragma once
#include <cstdint>

#include "Cartridge.h"

// Using this pixel engine as a display for the screen
// Thanks to javidx9 on Youtube for this
#include "olcPixelGameEngine.h"

class nes2C02
{
public:
	nes2C02();
	~nes2C02();

private: // Memories
	// VRAM to hold name table information (2kb's or two whole name tables of 1kb size)
	uint8_t tblName[2][1024];

	// VRAM for palette table (32 entries)
	uint8_t tblPalette[32];

	// Two 4kb arrays that represent pattern memory
	// This normally exists on the cartridge (not part of the PPU hardware)
	// This will be unused for base emulation but possibly used when making cool stuff with the emulator
	uint8_t tblPattern[2][4096];

private:
	// Array for all the colors the NES was cable of creating
	olc::Pixel palScreen[0x40];
	// This will represent the full screen output
	olc::Sprite sprScreen = olc::Sprite(256, 240);
	// This sprite will be a graphical representation of both nametables
	olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256, 240) };
	// This sprite will represent both patterntables
	olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) };


private: // Locals for registers and rendering
	// Pixel "dot" information
	int16_t scanline = 0;
	int16_t cycle = 0;

	// PPU Registers

	// PPUCTRL Register
	// Controller ($2000) > write
	union PPUCTRL
	{
		struct {
			// Base nametable address
			// (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00)
			uint8_t nametable_select : 2;	// Nametable select
			// VRAM address increment per CPU read/write of PPUDATA 
			// (0: add 1, going across; 1: add 32 going down)
			uint8_t increment_mode : 1;		// Incrememnt mode
			// Sprite pattern table address for 8x8 sprites (0: $0000, 1: $1000, ignored in 8x16 mode)
			uint8_t sprite_tile : 1;		// Sprite tile select
			// Background pattern table address (0: $0000, 1: $1000)
			uint8_t bg_tile : 1;			// BG tile select
			// Sprite size (0: 8x8 pixels; 1: 8x16 pixels)
			uint8_t sprite_height : 1;		// Sprite Height
			// (0: read backdrop from EXT pins: 1: output color on EXT pins) THIS WILL BE UNUSED
			uint8_t slave_switch : 1;		// PPU master/slave select
			// Generate an NMI at the start of the vertical blanking interval (0: off, 1: on)
			uint8_t nmi_enable : 1;			// NMI Enable
			
		};
		uint8_t reg;
		// Bits 1 and 0 are the most significant bit of the scrolling coordinates 
		// .... ..YX 
		// (Y = 1: Add 240 to the Y scroll position), (X = 1: Add 256 to X scroll)
		// This means when reaching the end of a nametable, we switch to the next one,
		// hence, changing the nametable address.

		// If the PPU is currently in v-blank, and the PPUSTATUS ($2002) vblank flag
		// is still set, changing the NMI flag in bit 7 from 0 to 1 will immediately
		// generate an NMI. This can result in graphical errors if the NMI routine
		// is executed too late in the blanking period to finish on time. To avoid
		// this problem it is prudent to read $2002 immediately before writing
		// $2000 to clear the vblank flag.
	} control;

	// Mask ($2001) > Write (PPUMASK)
	union PPUMASK {
		struct {
			uint8_t greyscale : 1;  // 0: normal color, 1: produce a greyscale display
			uint8_t left_bg : 1;	// 1: Show bg in leftmost 8 pixels of screen, 0: hide
			uint8_t left_sprites : 1; // 1: Show sprites in leftmost 8 pixels of screen, 0: hide
			uint8_t background : 1; // Show bg
			uint8_t sprites : 1;	// Show sprites
			uint8_t emph_red : 1;	// Emphasize red
			uint8_t emph_green : 1;
			uint8_t emph_blue : 1;
		};
		uint8_t reg;
	} mask;

	// Status ($2002) < Read (PPUSTATUS)
	union PPUSTATUS {
		struct {
			uint8_t unused : 5;

			// The intent for the flag was to be set whenever more than 8 sprites
			// appear on a scanline, but hardware bugs make it complicated.
			// Set during sprite evaluation and cleared at dot 1 of the pre-render line.
			uint8_t sprite_overflow : 1;

			// Sprite 0 hit. Set when a nonzero pixel of sprite 0 overlaps
			// a nonzero background pixel; cleared at dot 1 of the pre-render
			// line. Used for raster timing.
			uint8_t sprite0_hit : 1;

			// Vertical blank has started (0: not in vblank, 1: vblank).
			// Set at dot 1 of line 241 (the line *after* the post-render line);
			// cleared after reading $2002 and at dot 1 of the pre-render line.
			uint8_t vblank : 1;
			// Reading the status register will clear vblank and also the address latch 
			// used by PPUSCROLL and PPUADDR. Does not clear the others.
		};
		uint8_t reg;

	} status;

	// OAM Address ($2003) > write (OAMADDR)
	// Will do this when covering foreground rendering

	// OAM Data ($2004) <> read/write (OAMDATA)
	// Will do this when covering foreground rendering

	union loopy_register {
		// A guy named Loopy figured this out, not me
		struct {
			uint16_t coarse_x : 5;
			uint16_t coarse_y : 5;
			uint16_t nametable_x : 1;
			uint16_t nametable_y : 1;
			uint16_t fine_y : 3;
			uint16_t unused : 1;
		};
		uint16_t reg = 0x0000;
	};

	// Active "pointer" address into nametable to extract bg tile info
	loopy_register vram_addr;
	// Temporary store of information to be "transferred" into "pointer"
	loopy_register tram_addr;

	// Scroll ($2005) >> Write x2 (PPUSCROLL)
	// PPU Scrolling position register. It tells the PPU which pixel of the
	// nametable selected through PPUCTRL should be at the top left of the
	// rendered screen. Typically written to during vblanking so that the 
	// next frame starts rendering from the desired location.

	// Address $2006 >> Write x2 (PPUADDR)
	// Because the CPU and PPU are on separate buses, neither has direct access
	// to the other's memory. The CPU writes to VRAM through a pair of registers
	// on the PPU. First it loads to PPUADDR, and then writes repeatedly to 
	// PPUDATA to fill VRAM.
	// Valid addresses are $0000-$3FFF; higher addresses will be mirrored down

	// Pixel offset horizontally
	uint8_t fine_x = 0x00;

	// Internal communications
	uint8_t address_latch = 0x00; // Indicates whether on high or low byte of addr

	// When we read from PPU it is delayed by one cycle
	// so we will buffer the data read.
	uint8_t ppu_data_buffer = 0x00; 

	uint16_t ppu_address = 0x0000;

	// Background rendering

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
	void reset();
	bool nmi = false;

public:
	// Debugging Utilities
	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t i);
	olc::Sprite& GetPatternTable(uint8_t i, uint8_t palette);
	olc::Pixel& GetColorFromPaletteRam(uint8_t palette, uint8_t pixel);
	bool frame_complete = false;
};

