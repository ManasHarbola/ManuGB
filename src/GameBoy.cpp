#include "GameBoy.h"
#include <graphics/SDLDisplay.h>
#include <iostream>

GameBoy::GameBoy(std::string &rom_path, Display& display, Joypad& buttons) : 
int_manager_(), timer_(int_manager_), port_(int_manager_), ppu_(int_manager_),
mmu_(int_manager_, timer_, port_, ppu_, buttons), cpu_(mmu_, ppu_, timer_, int_manager_),
display_(display), buttons_(buttons) {
    //load rom onto mmu
    rom_loaded_ = mmu_.load_rom(rom_path);
    if (!rom_loaded_) {
        std::cout << "Loading rom " << rom_path << " failed..." << std::endl;
        return;
    }

    //attempt to load sram state 
    mmu_.load_state();
    buttons_.register_interrupt(int_manager_);
}

int GameBoy::run() {
    using namespace std::chrono;
    using std::this_thread::sleep_for;

    if (!rom_loaded_) {
        std::cout << "ManuGB cannot run - rom not loaded" << std::endl;
        return -1;
    }
    std::cout << "Beginning emulation..." << std::endl;
    #ifdef PROFILE
    double avg_fps = 0.0;
    size_t t = 1;
    #endif
    while (!display_.exit_requested()) {
        high_resolution_clock::time_point start = high_resolution_clock::now();
        
        bool was_paused = display_.pause_requested();
        display_.poll_events();
        if (!was_paused && display_.pause_requested())
            std::cout << std::endl << "Emulation Paused" << std::endl;
        else if (was_paused && !display_.pause_requested())
            std::cout << std::endl << "Emulation Resumed" << std::endl;

        if (display_.save_requested()) {
            //write save state to save file
            std::cout << std::endl << (mmu_.save_state() ? "Saving State Succeeded!" : "Saving State Failed!") << std::endl;
        }

        if (!display_.pause_requested()) {
            //update joypad
            buttons_.update();
            //step the emulator for CYCLES_PER_FRAME amount of t-cycles
            update(CYCLES_PER_FRAME);
            //draw framebuffer onto graphics window
            display_.draw((const void*)ppu_.get_buffer(), DISPLAY_WIDTH, DISPLAY_HEIGHT, 4);
        }

        high_resolution_clock::time_point end = high_resolution_clock::now();
        duration<double, std::milli> time_span = end - start;
        //60 frames / sec -> 16.66666 ms / frame
        double wait_msec = (1000.0 / 60.0) - time_span.count();
        #ifdef PROFILE
            //std::cout << "It took " << time_span.count() << "ms to perform " << CYCLES_PER_FRAME << " t-cycles" << std::endl;
            std::cout << "Unlocked FPS: " << static_cast<int>(1000.0 / time_span.count()) << std::endl;
            avg_fps += (static_cast<double>(1000.0 / time_span.count()) - avg_fps) / t;
            ++t;
            std::cout << "\t Average FPS: " << avg_fps << std::endl;
        #endif
        //delay to frame lock at 60fps
        if (wait_msec > 0)
            sleep_for(microseconds((int) (wait_msec * 1000)));
    }

    std::cout << std::endl << "Emulation terminated." << std::endl;

    return 0;
}

//the emulator cycles all the emulated hardware 1 t-cycle at a time for the sake of timing accuracy
inline int GameBoy::update(uint32_t num_cycles) {
    uint32_t cycles_completed = 0;
    while (cycles_completed < num_cycles) {
        cycles_completed += step();
    }
    return cycles_completed;
}

//update the state of every synchronous component by exactly 1 t-cycle
inline int GameBoy::step() {
    timer_.tick();
    port_.tick();
    cpu_.tick();
    mmu_.tick();
    ppu_.tick();
    return 1;
}