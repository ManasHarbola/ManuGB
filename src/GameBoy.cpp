#include "GameBoy.h"
#include <iostream>

GameBoy::GameBoy() {
    //initialize member variables here
}

int GameBoy::run() {
    std::cout << "Beginning emulation..." << std::endl;

    while(!exit_requested_) {
        //check for if pausing emulation is requested
        //this pausing isn't native to the console - it'll be exclusive to the emulator
        //paused_ = joypad_.pause_requested();
    
        if (!paused_) {
            //get start time from SDL
            //auto start = SDL_GetPerformanceCounter();

            //joypad_.update_state();

            //step the emulator for CYCLES_PER_FRAME amount of t-cycles
            update(CYCLES_PER_FRAME);

            //draw framebuffer onto graphics window

            //get end time for updating + rendering from SDL
            //auto end = SDL_GetPerformanceCounter();
            //auto elapsed = (end - start)....
            
            //delay remaining period of time to limit frame rate
            /*
            if (elasped > 1.0 / 60) {
                delay((1.0 / 60) - elapsed);
            }
            */
        }
    }

    return 0;
}

//the emulator cycles all the emulated hardware 1 t-cycle at a time for the sake of timing accuracy
void GameBoy::update(uint32_t num_cycles_requested) {
    uint32_t cycles_completed = 0;
    while (cycles_completed < num_cycles_requested) {
        cycles_completed += step();
    }
}

//update the state of every synchronous component by exactly 1 t-cycle
int GameBoy::step() {
    //timer_.tick();
    cpu_.tick();
    //dma_.tick();
    //port_.tick();
    //ppu_.tick();

    return 1;
}