#include "Timer.h"


Timer::Timer(InterruptManager &int_manager) : int_manager_(int_manager) {

}

void Timer::tick() {
    div_++;
    if (tac_ & TIMER_ENABLE) {
        //increment tima_ at appropriate frequency
        if (((div_ - 1) ^ (div_)) & (1 << freq_bit[tac_ & 0x3])) {
            tima_++;
            //check if timer overflowed
            if (tima_ == 0) {
                overflowed_ = true;
                ticks_since_overflow_ = 0;
            }
        }
        if (overflowed_) {
            ticks_since_overflow_++;
            if (ticks_since_overflow_ == 4)
                int_manager_.request_interrupt(InterruptType::TIMER);
            else if (ticks_since_overflow_ == 5)
                tima_ = tma_;
            else if (ticks_since_overflow_ == 6) {
                tima_ = tma_;
                overflowed_ = false;
                ticks_since_overflow_ = 0;
            }
        }
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
        return (tac_ | 0xF8);
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
        div_ = 0;
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
