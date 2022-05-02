#pragma once
#include <cstdint>
#include <array>
#include "nes6502.h"

class Bus
{
public:
	Bus();
	~Bus();

public: // Devices on CPU
	nes6502 cpu; // our CPU

	// Fake 64kb RAM
	std::array<uint8_t, 64 * 1024> ram;

public: // Bus Read and Write
	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr, bool bReadOnly = false);
};

