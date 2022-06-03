#include "Timer.h"


Timer::Timer(InterruptManager &int_manager) : int_manager_(int_manager) {

}

void Timer::tick() {
    set_div(div_ + 1);
    if (!overflowed_)
        return;
    
    ticks_since_overflow_++;
    if (ticks_since_overflow_ == 4)
        int_manager_.request_interrupt(InterruptType::TIMER);
    if (ticks_since_overflow_ == 5)
        tima_ = tma_;
    if (ticks_since_overflow_ == 6) {
        tima_ = tma_;
        overflowed_ = false;
        ticks_since_overflow_ = 0;
    }
    
}

uint8_t Timer::read(uint16_t addr) {
    switch (addr)
    {
    case DIV:
        return (uint8_t) (div_ >> 8);
        break;
    case TIMA:
        return tima_;
        break;
    case TMA:
        return tma_;
        break;
    case TAC:
        return tac_ | 0xF8;
        break;
    default:
        return 0xFF;
        break;
    }
}

void Timer::write(uint16_t addr, uint8_t val) {
    switch (addr)
    {
    case DIV:
        set_div(0);
        break;
    case TIMA:
        if (ticks_since_overflow_ < 5) {
            tima_ = val;
            overflowed_ = false;
            ticks_since_overflow_ = 0;
        }
        break;
    case TMA:
        tma_ = val;
        break;
    case TAC:
        tac_ = val;
        break;
    default:
        break;
    }
}

void Timer::inc_tima() {
    tima_++;
    //check for overflow 
    if (tima_ == 0) {
        overflowed_ = true;
        ticks_since_overflow_ = 0;
    }
}

void Timer::set_div(uint16_t div) {
    div_ = div;
    uint8_t pos = falling_edge_bit[tac_ & 0x3];
    bool bit_val = (div_ & (1 << pos)) != 0;
    //check for falling edge (prev_bit was 1 and bit_val is 0)
    if (!bit_val && prev_bit_) {
        inc_tima();
    }
    prev_bit_ = bit_val;
}

