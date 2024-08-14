#pragma once

#include <array>
#include <cstdint>

#include "CPU6502.h"

using namespace std;

class Bus {
   public:
    Bus();
    ~Bus();

   public:
    CPU6502 cpu;
    array<uint8_t, 64 * 1024> ram;

    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool bReadOnly = false);
};
