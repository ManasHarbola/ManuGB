#include <iostream>
#include <string>
#include "GameBoy.h"
#include <graphics/SDLDisplay.h>
#include <cstdlib>

void tweakBuffer(char *buffer, size_t sz) {
    for (int i = 0; i < sz; i++) {
        buffer[i] = std::rand() % sz;
    }
}

int main(int argc, char **argv) {
    //currently, we just need to implement the CPU, we can worry about file handle to the ROM later during testing
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << SDL_GetError() << std::endl;
        //errcode = - 1;
        return -1;
    }    

    std::string rom_path = "./tests/";

    rom_path += std::string(argv[1]);

    int display_errcode = 0;
    SDLDisplay disp("ManuGB: " + rom_path, 640, 480, display_errcode);
    if (display_errcode < 0) {
        std::cout << "Issue Encountered when setting up SDL Display" << std::endl;
        return -1;
    }
    GameBoy emulator(rom_path, disp);

    emulator.run();
    SDL_Quit();
}