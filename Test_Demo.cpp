#include <iostream>
#include <sstream>

#include "Bus.h"
#include "nes6502.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Demo_nes6502 : public olc::PixelGameEngine
{
public:
	Demo_nes6502() { sAppName = "PPU Background Demonstration"; }

	std::shared_ptr<Cartridge> cart;
	Bus nes;
	
	// If true, run the emulation in real time
	bool bEmulationRun = false;
	float fResidualTime = 0.0f;

	// Used to let user choose palette
	uint8_t nSelectedPalette = 0x00;
	uint16_t ram_line = 0x2000;

private:
	std::map<uint16_t, std::string> mapAsm;

	std::string hex(uint32_t n, uint8_t d) {
		std::string s(d, '0');
		// n >>= 4 means n = right shift 4 (n = n >> 4)
		for (int i = d - 1; i >= 0; i--, n >>= 4) {
			s[i] = "0123456789ABCDEF"[n & 0xF]; // n & 0xF is equivalent to n % 16
		}
		return s;
	};

	void drawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns) {
		int nRamX = x, nRamY = y;
		for (int row = 0; row < nRows; row++) {
			std::string sOffset = "$" + hex(nAddr, 4) + ":";
			for (int col = 0; col < nColumns; col++) {
				sOffset += " " + hex(nes.cpuRead(nAddr, true), 2);
				nAddr += 1;
			}
			DrawString(nRamX, nRamY, sOffset);
			nRamY += 10;
		}
	}

	void drawCPU(int x, int y) {
		std::string status = "STATUS: ";
		DrawString(x, y, "STATUS:", olc::WHITE);
		DrawString(x + 64, y, "N", nes.cpu.status & nes6502::N ? olc::GREEN : olc::RED);
		DrawString(x + 80, y, "V", nes.cpu.status & nes6502::V ? olc::GREEN : olc::RED);
		DrawString(x + 96, y, "-", nes.cpu.status & nes6502::U ? olc::GREEN : olc::RED);
		DrawString(x + 112, y, "B", nes.cpu.status & nes6502::B ? olc::GREEN : olc::RED);
		DrawString(x + 128, y, "D", nes.cpu.status & nes6502::D ? olc::GREEN : olc::RED);
		DrawString(x + 144, y, "I", nes.cpu.status & nes6502::I ? olc::GREEN : olc::RED);
		DrawString(x + 160, y, "Z", nes.cpu.status & nes6502::Z ? olc::GREEN : olc::RED);
		DrawString(x + 178, y, "C", nes.cpu.status & nes6502::C ? olc::GREEN : olc::RED);
		DrawString(x, y + 10, "PC: $" + hex(nes.cpu.pc, 4));
		DrawString(x, y + 20, "A: $" + hex(nes.cpu.a, 2) + "  [" + std::to_string(nes.cpu.a) + "]");
		DrawString(x, y + 30, "X: $" + hex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]");
		DrawString(x, y + 40, "Y: $" + hex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]");
		DrawString(x, y + 50, "Stack P: $" + hex(nes.cpu.stkp, 4));
	}

	void drawCode(int x, int y, int nLines) {
		// find() returns an iterator to the position where the key is in the map
		auto it_a = mapAsm.find(nes.cpu.pc);
		int nLineY = (nLines >> 1) * 10 + y;
		if (it_a != mapAsm.end()) {
			DrawString(x, nLineY, it_a->second, olc::CYAN);
			while (nLineY < (nLines * 10) + y) {
				nLineY += 10;
				if (++it_a != mapAsm.end()) {
					DrawString(x, nLineY, it_a->second);
					// it_a++;
				}
			}
		}

		it_a = mapAsm.find(nes.cpu.pc);
		nLineY = (nLines >> 1) * 10 + y;
		if (it_a != mapAsm.end()) {
			while (nLineY > y) {
				nLineY -= 10;
				if (--it_a != mapAsm.end()) {
					DrawString(x, nLineY, it_a->second);
				}
			}
		}
	}

	bool OnUserCreate() {
		// Load the cartridge by creating the shared ptr object
		cart = std::make_shared<Cartridge>("roms/SMB.nes");

		//if (!cart->ImageValid())
		//	return false;

		// Insert cartridge
		nes.insertCartridge(cart);

		// Extract assembly over the whole range
		mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

		// Reset NES
		nes.reset();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) {
		Clear(olc::DARK_BLUE);

		// Reset on R key
		if (GetKey(olc::Key::R).bPressed) nes.reset();
		if (GetKey(olc::Key::SPACE).bPressed) bEmulationRun = !bEmulationRun;
		if (GetKey(olc::Key::P).bPressed) (++nSelectedPalette) &= 0x07;
		if (GetKey(olc::Key::DOWN).bPressed) {
			ram_line += 0x0040;
		}
		if (GetKey(olc::Key::UP).bPressed) {
			ram_line -= 0x0040;
			if (ram_line < 0) {
				ram_line = 0x0000;
			}
		}

		if (bEmulationRun) {
			if (fResidualTime > 0.0f)
				fResidualTime -= fElapsedTime;
			else {
				fResidualTime += (1.0f / 60.0f) - fElapsedTime;
				do { nes.clock(); } while (!nes.ppu.frame_complete);
				nes.ppu.frame_complete = false;
			}
		}
		else {
			// Emulate code step-by-step
			if (GetKey(olc::Key::C).bPressed) {
				// Clock enough times for a whole instruction
				do { nes.clock(); } while (!nes.cpu.complete());
				// CPU clock runs slower than the system clock, so it may be
				// complete for additional system clock cycles.
				// Drain those out.
				do { nes.clock(); } while (nes.cpu.complete());
			}

			if (GetKey(olc::Key::N).bPressed) {
				for (int i = 0; i < 10; i++) {
					do { nes.clock(); } while (!nes.cpu.complete());
					do { nes.clock(); } while (nes.cpu.complete());
				}
			}
			
			// Emulate one whole frame
			if (GetKey(olc::Key::F).bPressed) {
				// Clock enough times to draw a single frame
				do { nes.clock(); } while (!nes.ppu.frame_complete);
				// use residual clock cycles to complete current instruction
				do { nes.clock(); } while (!nes.cpu.complete());
				// Reset frame completion flag
				nes.ppu.frame_complete = false;
			}
		}


		drawCPU(516, 2);
		drawCode(516, 72, 26);
		// drawRam(2, 2, ram_line, 32, 16);

		// Draw palettes and pattern tables
		const int nSwatchSize = 6; // ??
		for (int p = 0; p < 8; p++) // for each palette
			for (int s = 0; s < 4; s++) // for each index
				FillRect(516 + p * (nSwatchSize * 5) + s * nSwatchSize, 340,
					nSwatchSize, nSwatchSize, nes.ppu.GetColorFromPaletteRam(p, s));

		// Draw slection reticule around selected palette
		DrawRect(516 + nSelectedPalette * (nSwatchSize * 5) - 1, 339, (nSwatchSize * 4), nSwatchSize, olc::WHITE);

		// Draw left and then right pattern table
		DrawSprite(516, 348, &nes.ppu.GetPatternTable(0, nSelectedPalette));
		DrawSprite(648, 348, &nes.ppu.GetPatternTable(1, nSelectedPalette));

		// Draw rendered output
		DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);

		// Draw ID's in the corresponding nametable locations
		//for (uint8_t y = 0; y < 30; y++) {
		//	for (uint8_t x = 0; x < 32; x++) {
		//		// there's a bug for tblname that's requiring me to public the variable
		//		DrawString(x * 16, y * 16, hex((uint32_t)nes.ppu.tblName[0][y * 32 + x], 2));
		//	}
		//}

		return true;
	}
};

int main() {
	Demo_nes6502 demo;
	demo.Construct(780, 480, 2, 2);
	demo.Start();
	return 0;
}