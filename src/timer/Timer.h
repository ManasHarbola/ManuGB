#pragma once

#include "../Logging.h"
#include "../cpu/InterruptManager.h"
#include "../Constants.h"

#define DIV 0xFF04
#define TIMA 0xFF05
#define TMA 0xFF06
#define TAC 0xFF07
#define TIMER_ENABLE (1 << 2)

class Timer {
public:
    Timer(InterruptManager& int_manager);    
    void tick();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t val);
private:
    InterruptManager& int_manager_;
    uint16_t div_{0x0000};
    uint8_t tima_{0x00}, tma_{0x00}, tac_{0x00};
    bool overflowed_{false};
    uint16_t ticks_since_overflow_{0x0000};
    static constexpr uint8_t freq_bit[]{10,4,6,8};
};