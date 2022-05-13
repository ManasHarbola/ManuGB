#include "CPU.h"

CPU::CPU() {
    //initialize register, flag register, sp, and pc
    registers_.af = 0x01B0;
    registers_.bc = 0x0013;
    registers_.de = 0x00D8;
    registers_.hl = 0x014D;
    registers_.sp = 0xFFFE;
    registers_.pc = 0x0100;

    flag_register_ = 0x00;
}

void CPU::tick() {
    return;
}