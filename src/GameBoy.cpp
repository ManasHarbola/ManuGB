#include "GameBoy.h"
#include <graphics/SDLDisplay.h>
#include <iostream>

GameBoy::GameBoy(std::string &rom_path, Display& display, Joypad& buttons) : 
int_manager_(), timer_(int_manager_), port_(int_manager_), ppu_(int_manager_),
mmu_(int_manager_, timer_, port_, ppu_, buttons), cpu_(mmu_, ppu_, timer_, int_manager_),
display_(display), buttons_(buttons) {
    //initialize member variables here
    //load rom onto mmu
    rom_loaded_ = load_rom(rom_path);
    if (!rom_loaded_) {
        std::cout << "Loading rom " << rom_path << " failed..." << std::endl;
        return;
    }
    buttons_.register_interrupt(int_manager_);
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
    
    //SDL_Event e;

    //while(!exit_requested_) {
    while (!display_.exit_requested()) {
        //check for if pausing emulation is requested
        if (!paused_) {
            high_resolution_clock::time_point start = high_resolution_clock::now();

            display_.poll_events();
            buttons_.update();
            //step the emulator for CYCLES_PER_FRAME amount of t-cycles
            update(CYCLES_PER_FRAME);
            //draw framebuffer onto graphics window
            display_.draw((const void*)ppu_.get_buffer(), DISPLAY_WIDTH, DISPLAY_HEIGHT, 4);

            //delay to frame lock at 60fps
            high_resolution_clock::time_point end = high_resolution_clock::now();
            duration<double, std::milli> time_span = end - start;
            //60 frames / sec -> 16.66666 ms / frame
            double wait_msec = (1000.0 / 60.0) - time_span.count();
            
            //std::cout << "It took " << time_span.count() << "ms to perform " << CYCLES_PER_FRAME << " t-cycles" << std::endl;
            if (wait_msec > 0) {
                sleep_for(microseconds((int) (wait_msec * 1000)));
            }

        }
    }

    std::cout << "\nEmulation terminated." << std::endl;

    return 0;
}

//the emulator cycles all the emulated hardware 1 t-cycle at a time for the sake of timing accuracy
int GameBoy::update(uint32_t num_cycles) {
    uint32_t cycles_completed = 0;
    while (cycles_completed < num_cycles) {
        cycles_completed += step();
    }
    return cycles_completed;
}

//update the state of every synchronous component by exactly 1 t-cycle
int GameBoy::step() {
    timer_.tick();
    //buttons_.tick();
    port_.tick();
    cpu_.tick();
    mmu_.tick();
    ppu_.tick();
    return 1;
}