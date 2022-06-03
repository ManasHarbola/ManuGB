#include "GameBoy.h"
#include <iostream>

GameBoy::GameBoy(std::string &rom_path) : 
int_manager_(), timer_(int_manager_), ppu_(),
mmu_(int_manager_, timer_, ppu_), cpu_(mmu_, ppu_, timer_, int_manager_) {
    //initialize member variables here
    //load rom onto mmu
    rom_loaded_ = load_rom(rom_path);
    if (!rom_loaded_) {
        std::cout << "Loading rom " << rom_path << " failed..." << std::endl;
        return;
    }
}

bool GameBoy::load_rom(std::string &rom_path) {
    return mmu_.load_rom(rom_path);
}

int GameBoy::run() {
    using namespace std::chrono;
    using std::this_thread::sleep_for;

    if (!rom_loaded_) {
        std::cout << "ManuGB cannot run - rom not loaded" << std::endl;
        return -1;
    }

    std::cout << "Beginning emulation..." << std::endl;
    std::cout << "Loading rom..." << std::endl;


    while(!exit_requested_) {
        //check for if pausing emulation is requested
        //this pausing isn't native to the console - it'll be exclusive to the emulator
        //paused_ = joypad_.pause_requested();
    
        if (!paused_) {
            //get start time from SDL
            //auto start = SDL_GetPerformanceCounter();

            high_resolution_clock::time_point start = high_resolution_clock::now();

            //joypad_.update_state();

            //step the emulator for CYCLES_PER_FRAME amount of t-cycles
            update(CYCLES_PER_FRAME);

            //draw framebuffer onto graphics window

            //get end time for updating + rendering from SDL
            //auto end = SDL_GetPerformanceCounter();
            //auto elapsed = (end - start)....
            
            high_resolution_clock::time_point end = high_resolution_clock::now();
            duration<double, std::milli> time_span = end - start;
            //60 frames / sec -> 16.66666 ms / frame
            double wait_msec = (1000.0 / 60.0) - time_span.count();

            //std::cout << "It took " << wait_msec << "ms to perform " << CYCLES_PER_FRAME << " t-cycles" << std::endl;
            if (wait_msec > 0) {
                sleep_for(microseconds((int) (wait_msec * 1000)));
            }

        }
    }

    std::cout << "Emulation terminated." << std::endl;

    return 0;
}

//the emulator cycles all the emulated hardware 1 t-cycle at a time for the sake of timing accuracy
int GameBoy::update(uint32_t num_cycles_requested) {
    uint32_t cycles_completed = 0;
    while (cycles_completed < num_cycles_requested) {
        cycles_completed += step();
    }
    return cycles_completed;
}

//update the state of every synchronous component by exactly 1 t-cycle
int GameBoy::step() {
    timer_.tick();
    cpu_.tick();
    //dma_.tick();
    //port_.tick();
    //ppu_.tick();

    return 1;
}