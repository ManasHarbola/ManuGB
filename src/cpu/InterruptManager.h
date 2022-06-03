#pragma once

#include "../Constants.h"
#include <unordered_map>

enum InterruptType {
    NONE = 0x00,
    VBLANK = 0x01,
    LCDSTAT = 0x02,
    TIMER = 0x04,
    SERIAL = 0x08,
    JOYPAD = 0x10,   
};

class InterruptManager {
public:
    bool interrupt_requested();
    bool halt_bug_encountered();
    InterruptType get_requested_interrupt();
    void request_interrupt(InterruptType interrupt);
    void clear_interrupt(InterruptType interrupt);
    void disable_IME();
    void set_IE(uint8_t ie);
    void set_IF(uint8_t new_if);

    void enable_IME(bool extra_cycle=false);

    uint8_t get_IF() {return IF;}
    uint8_t get_IE() {return IE;}

    bool get_IME();
    bool pending_enable_IME();
    static const std::unordered_map<InterruptType, uint16_t> isr_addrs;
private:
    uint8_t IF{0xE1}; 
    uint8_t IE{0x00};
    bool IME{false};
    bool pending_IME_enable_{false};
};