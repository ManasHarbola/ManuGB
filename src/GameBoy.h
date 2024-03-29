#pragma once

#include <SDL2/SDL.h>
#include <chrono>
#include <thread>
#include "cpu/CPU.h"
#include "ppu/PPU.h"
#include "timer/Timer.h"
#include "mmu/MMU.h"
#include "serial/Serial.h"
#include "cpu/InterruptManager.h"
#include <joypad/Joypad.h>
#include <graphics/Display.h>
#include "Constants.h"
#include <string>



//entry point into beginning emulation of rom
class GameBoy {
    public:
        GameBoy(std::string &rom_path, Display& display, Joypad& buttons);
        //bool load_rom(std::string &rom_path);
        int run();
        //steps all our hardware by specified number of t-cycles
        int update(uint32_t num_cycles);
        //steps all our hardware by 1 t-cycle    
        int step();    
    private:
        InterruptManager int_manager_;
        Timer timer_;
        Serial port_;
        PPU ppu_;
        MMU mmu_;
        CPU cpu_;

        Display& display_;
        Joypad& buttons_;
        
        bool rom_loaded_{false};
};
