#pragma once

#include <cstdint>
#include <cpu/InterruptManager.h>
#include <mmu/AddressSpace.h>

class Joypad : public AddressSpace {
public:
    static constexpr uint8_t SELECT_DIRECTION{1 << 4};
    static constexpr uint8_t SELECT_ACTION{1 << 5};
    virtual void poll_joypad() = 0;
    void update() {
        uint8_t prev_action = action_;
        uint8_t prev_dpad = dpad_;
        poll_joypad();
        //check if joypad interrupt has to be requested
        for (uint8_t shift = 0x08; shift != 0; shift >>= 1) {
            if ((select_ == 0xD0 && ((prev_action & shift) && !(action_ & shift))) || 
                (select_ == 0xE0 && ((prev_dpad & shift) && !(dpad_ & shift)))) {
                //std::cout << "press detected on shift " << (int) shift << std::endl;
                if (int_manager_)
                    int_manager_->request_interrupt(InterruptType::JOYPAD);
                break;
            } 
        }
    }
    void register_interrupt(InterruptManager& int_manager) {
        if (!int_manager_)
            int_manager_ = &int_manager;
    }
    uint8_t read(uint16_t addr) override {
        if (addr == 0xFF00) {
            switch (select_) {
                case 0xD0:
                    return select_ | action_;
                    break;
                case 0xE0:
                    return select_ | dpad_;
                    break;
                default:
                    return 0xCF;
            }
        }
        return 0xFF;
    }
    void write(uint16_t addr, uint8_t val) override {
        if (addr == 0xFF00)
            select_ = 0xC0 | (val & 0x30);
    }
    bool manages(uint16_t addr) override {
        return addr == 0xFF00;
    }
protected:
    uint8_t select_{0xC0};
    uint8_t dpad_{0x0F};
    uint8_t action_{0x0F};
private:
    InterruptManager* int_manager_{nullptr};

};