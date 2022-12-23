#include <iostream>
#include <string>
#include "GameBoy.h"

int main(int argc, char **argv) {
    //currently, we just need to implement the CPU, we can worry about file handle to the ROM later during testing
    
    //std::string rom_path = "./tests/gb-test-roms/cpu_instrs/individual/01-special.gb";
    std::string rom_path = "./tests/";
    //std::string rom_path = "./tests/gb-test-roms/instr_timing/";

    rom_path += std::string(argv[1]);

    GameBoy emulator(rom_path);

    emulator.run();
}