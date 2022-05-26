#pragma once

#include "../cpu/InterruptManager.h"
#include "../timer/Timer.h"
#include "../ppu/PPU.h"
#include "../Constants.h"
#include <string>
#include <string.h>
#include <cstring>
#include <fstream>
#include <iostream>

#define ROM_SPACE 65536
#define CART_SPACE 0x200000

class MMU {
    public:
        MMU(InterruptManager &int_manager, Timer &timer, PPU &ppu);
        bool load_rom(const std::string &rom_path);
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t val);
    private:
        //uint8_t curr_bank_{1};
        uint8_t mem_[ROM_SPACE];
        uint8_t cart_[CART_SPACE];
        InterruptManager int_manager_;
        Timer &timer_;
        PPU &ppu_;


};