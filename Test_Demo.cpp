#include <iostream>
#include <sstream>

#include "Bus.h"
#include "nes6502.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Demo_nes6502 : public olc::PixelGameEngine
{
public:
	Demo_nes6502() { sAppName = "nes6502 Demonstration"; }

	std::shared_ptr<Cartridge> cart;
	Bus nes;
	std::map<uint16_t, std::string> mapAsm;

	// If true, going to try to run the emulation in real time
	bool bEmulationRun = false;
	float fResidualTime = 0.0f;

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
				}
			}
		}

		it_a = mapAsm.find(nes.cpu.pc);
		// maybe give some thought into understanding this later
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
		// ROM/MAPPER DEMO

		// Load the cartridge by creating the shared ptr object
		cart = std::make_shared<Cartridge>("nestest.nes");

		// Insert cartridge
		nes.insertCartidge(cart);

		// Extract assembly over the whole range
		mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

		// Reset NES
		nes.reset();

		return true;
		// CPU TEST DEMO
		// Load Program (assembled at https://www.masswerk.at/6502/assembler.html)
		// Its purpose is to test the assembly code
		// This will multiply 10x3
		/*
			*=$8000
			LDX #10		/* Load X register with number 10
			STX $0000	/* Store X register at address 0000
			LDX #3		/* Load X register with number 3
			STX $0001	/* Store X reg value at addr 0001
			LDY $0000	/* Store value at $0000 to Y reg
			LDA #0		/* Load accumulator with 0 value
			CLC			/* Clear the carry bit
			loop
			ADC $0001   /* Adds value at $0001 to accumulator
			DEC $0000	/* Decrememnts value at $0000
			BNE loop	/* Branch to loop unless Z flag is set due to DEC
			STA $0002	/* Store the accumulator at $0002
		*/

		//std::stringstream ss;
		//ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
		//uint16_t nOffset = 0x8000;
		//while (!ss.eof()) {
		//	std::string b;
		//	ss >> b;
		//	nes.cpuRam[nOffset++] = (uint8_t)std::stoul(b, nullptr, 16);
		//}

		//// Set Reset Vector
		//nes.cpuRam[0xFFFC] = 0x00;
		//nes.cpuRam[0xFFFD] = 0x80;

		//// Don't forget to set IRQ and NMI vectors when testing those

		//// Extract disassembly
		//mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

		//// Reset
		//nes.cpu.reset();
		//return true;
	}

	bool OnUserUpdate(float fElapsedTime) {
		Clear(olc::DARK_BLUE);

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

		// Reset on R key
		if (GetKey(olc::Key::R).bPressed) nes.reset();
		if (GetKey(olc::Key::SPACE).bPressed) bEmulationRun = !bEmulationRun;

		drawCPU(516, 2);
		drawCode(516, 72, 26);

		DrawSprite(0, 0, &nes.ppu.GetScreen(), 2);

		// CPU Test/Demo
		//if (GetKey(olc::Key::SPACE).bPressed) {
		//	do {
		//		nes.cpu.clock();
		//	} while (!nes.cpu.complete());
		//}

		//if (GetKey(olc::Key::R).bPressed)
		//	nes.cpu.reset();

		//if (GetKey(olc::Key::I).bPressed)
		//	nes.cpu.irq();

		//if (GetKey(olc::Key::N).bPressed)
		//	nes.cpu.nmi();

		//// Draw Ram Page 0x00
		//drawRam(2, 2, 0x0000, 16, 16);
		//drawRam(2, 182, 0x1000, 16, 16);
		//drawCPU(448, 2);
		//drawCode(448, 72, 26);

		//DrawString(10, 370, "SPACE = Step Instruction    R = Reset    I = IRQ    N = NMI");

		return true;
	}
};

int main() {
	Demo_nes6502 demo;
	demo.Construct(800, 480, 2, 2);
	demo.Start();
	return 0;
}