#pragma once

#include <SDL2/SDL.h>
#include "cpu/CPU.h"
#include "ppu/PPU.h"
#include "timer/Timer.h"
#include "UI/ControlManager.h"
#include "mmu/MMU.h"
#include "DMA.h"
#include "serial/Serial.h"
#include "cpu/InterruptManager.h"

#define CLOCK_FREQ 4194304
#define CYCLES_PER_FRAME 69905

//entry point into beginning emulation of rom
class GameBoy {
    public:
        int run();
        //steps all our hardware by specified number of t-cycles
        int update(uint32_t num_cycles_requested);
        //steps all our hardware by 1 t-cycle    
        int step();    
    private:
        CPU cpu_;
        PPU ppu_;
        Timer timer_;
        ControlManager joypad_;
        MMU mmu_;
        DMA dma_;
        Serial port_;
        InterruptManager int_handler_;

        bool paused_{false};
        bool exit_requested_{false};
};