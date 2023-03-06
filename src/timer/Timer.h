#pragma once

#include "../Logging.h"
#include "../cpu/InterruptManager.h"
#include "../Constants.h"
#include <mmu/AddressSpace.h>

class Timer : public AddressSpace {
public:
    Timer(InterruptManager& int_manager);    
    void tick();
    uint8_t read(uint16_t addr) override;
    void write(uint16_t addr, uint8_t val) override;
    bool manages(uint16_t addr) override {
        return (addr >= DIV && addr <= TAC);
    }

    static constexpr uint16_t DIV{0xFF04};
    static constexpr uint16_t TIMA{0xFF05};
    static constexpr uint16_t TMA{0xFF06};
    static constexpr uint16_t TAC{0xFF07};
    static constexpr uint8_t TIMER_ENABLE{1 << 2};   

private:
    InterruptManager& int_manager_;
    uint16_t div_{0};
    uint8_t tima_{0}, tma_{0}, tac_{0};
    bool overflowed_{false};
    uint16_t ticks_since_overflow_{0};
    static constexpr uint8_t freq_bit[]{10,4,6,8};
};