#pragma once

#include <unordered_map>

enum InterruptType {
    VBLANK = 0x01,
    LCDSTAT = 0x02,
    TIMER = 0x04,
    SERIAL = 0x08,
    JOYPAD = 0x10,   
};

class InterruptManager {
public:
    bool interrupt_requested();
    InterruptType get_requested_interrupt();
    void request_interrupt(InterruptType interrupt);
    void disable_IME();
    void set_IE(uint8_t ie);
    void enable_IME(bool extra_cycle=false);

    bool get_IME();
    bool pending_enable_IME();
    static const std::unordered_map<InterruptType, uint16_t> isr_addrs;
private:
    uint8_t IF{0xE1}, IE{0x00};
    bool IME{false};
    bool pending_IME_enable{false};
};