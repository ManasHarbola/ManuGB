#include "InterruptManager.h"
//#include <iostream>

const std::unordered_map<InterruptType, uint16_t> InterruptManager::isr_addrs = {
    {VBLANK, 0x0040},
    {LCDSTAT, 0x0048},
    {TIMER, 0x0050},
    {SERIAL, 0x0058},
    {JOYPAD, 0x0060}
};

bool InterruptManager::interrupt_requested() {
    //return (IF & IE) != 0;
    return (IF & IE & 0x1F) != 0;
}

bool InterruptManager::halt_bug_encountered() {
    return interrupt_requested() && !IME;
}

InterruptType InterruptManager::get_requested_interrupt() {
    uint8_t flags = IF & IE & 0x1F;
    if (flags & (uint8_t)(VBLANK)) return VBLANK;
    if (flags & (uint8_t)(LCDSTAT)) return LCDSTAT;
    if (flags & (uint8_t)(TIMER)) return TIMER;
    if (flags & (uint8_t)(SERIAL)) return SERIAL;
    if (flags & (uint8_t)(JOYPAD)) return JOYPAD;

    return NONE;
}

bool InterruptManager::get_IME() {
    return IME;
}

bool InterruptManager::pending_enable_IME() {
    return pending_IME_enable_;
}

void InterruptManager::request_interrupt(InterruptType interrupt) {
    IF |= (uint8_t)(interrupt);
}
void InterruptManager::clear_interrupt(InterruptType interrupt) {
    IF &= ~((uint8_t) interrupt);
}

void InterruptManager::set_IE(uint8_t ie) {
    IE = ie;
}

void InterruptManager::set_IF(uint8_t new_if) {
    IF = (new_if | 0xE0);
}

void InterruptManager::disable_IME(){
    pending_IME_enable_ = false;
    IME = false;
}

void InterruptManager::enable_IME(bool extra_cycle) {
    if(extra_cycle) {
        pending_IME_enable_ = true;
        return;
    }
    pending_IME_enable_ = false;
    IME = true;
}
