#pragma once
#include "../cpu/InterruptManager.h"
#include "../Constants.h"

#define DIV 0xFF04
#define TIMA 0xFF05
#define TMA 0xFF06
#define TAC 0xFF07

#define TIMER_ENABLE (1 << 2)

class Timer {
public:
    Timer(InterruptManager &int_manager);    
    //Steps timer forward by 1 t-cycle
    void tick();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t val);

private:
    void set_div(uint16_t div);
    void inc_tima();

    InterruptManager &int_manager_;

    uint16_t div_{0x0000};
    uint8_t tima_{0x00}, tma_{0x00}, tac_{0x00};
    bool prev_bit_{false};
    bool overflowed_{false};
    uint16_t ticks_since_overflow_{0x0000};
    static constexpr uint8_t falling_edge_bit[]{9,3,5,7};
};