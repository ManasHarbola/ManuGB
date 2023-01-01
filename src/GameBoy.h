#pragma once

#include <SDL2/SDL.h>
#include <chrono>
#include <thread>
#include "cpu/CPU.h"
#include "ppu/PPU.h"
#include "timer/Timer.h"
#include "UI/ControlManager.h"
#include "mmu/MMU.h"
#include "serial/Serial.h"
#include "cpu/InterruptManager.h"
#include <graphics/Display.h>
#include "Constants.h"
#include <string>



//entry point into beginning emulation of rom
class GameBoy {
    public:
        GameBoy(std::string &rom_path, Display& display);
        bool load_rom(std::string &rom_path);
        int run();
        //steps all our hardware by specified number of t-cycles
        int update(uint32_t num_cycles);
        //steps all our hardware by 1 t-cycle    
        int step();    
    private:
        InterruptManager int_manager_;
        Timer timer_;
        PPU ppu_;
        MMU mmu_;
        CPU cpu_;

        Display& display_;
        //ControlManager joypad_;
        /*
        DMA dma_;
        Serial port_;
        */

        bool rom_loaded_{false};
        bool paused_{false};
        bool exit_requested_{false};
};
