#include "InterruptManager.h"

std::unordered_map<InterruptType, uint16_t> InterruptManager::isr_addr = {
    {VBLANK, 0x0040},
    {LCDSTAT, 0x0048},
    {TIMER, 0x0050},
    {SERIAL, 0x0058},
    {JOYPAD, 0x0060}
};

bool InterruptManager::interrupt_requested() {
    return (IF & IE) != 0;
}

bool InterruptManager::halt_bug_encountered() {
    return ((IE & IF & 0x1F) != 0) && !IME;
}

InterruptType InterruptManager::get_requested_interrupt() {
    if (IF & IE & (uint8_t)(VBLANK)) return VBLANK;
    if (IF & IE & (uint8_t)(LCDSTAT)) return LCDSTAT;
    if (IF & IE & (uint8_t)(TIMER)) return TIMER;
    if (IF & IE & (uint8_t)(SERIAL)) return SERIAL;
    if (IF & IE & (uint8_t)(JOYPAD)) return JOYPAD;
}

bool InterruptManager::get_IME() {
    return IME;
}

bool InterruptManager::pending_enable_IME() {
    return pending_IME_enable;
}

void InterruptManager::request_interrupt(InterruptType interrupt) {
    IF |= (uint8_t)(interrupt);
}

void InterruptManager::set_IE(uint8_t ie) {
    IE = ie;
}

void InterruptManager::disable_IME(){
    pending_IME_enable = false;
    IME = false;
}

void InterruptManager::enable_IME(bool extra_cycle=false) {
    if(extra_cycle) {
        pending_IME_enable = true;
        return;
    }
    pending_IME_enable = false;
    IME = true;
}
