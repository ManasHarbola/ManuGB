#include <iostream>
#include "GameBoy.h"

int main(int argc, char **argv) {
    //currently, we just need to implement the CPU, we can worry about file handle to the ROM later during testing
    GameBoy emulator;

    emulator.run();
}