#include <iostream>
#include <string>
#include "GameBoy.h"
#include <graphics/SDLDisplay.h>
#include <cstdlib>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "ROM File Not Specified" << std::endl;
        return -1;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << SDL_GetError() << std::endl;
        return -1;
    }    
    std::string rom_path = "./" + std::string(argv[1]);

    int display_errcode = 0;

    //SDLDisplay is both an instance of Display and Joypad
    SDLDisplay disp("ManuGB: " + rom_path, 960, 864, display_errcode);
    if (display_errcode < 0) {
        std::cout << "Issue Encountered when setting up SDL Display" << std::endl;
        return -1;
    }
    GameBoy emulator(rom_path, disp, disp);
    emulator.run();
    SDL_Quit();
}