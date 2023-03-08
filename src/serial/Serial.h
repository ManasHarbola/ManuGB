#pragma once

#include <Logging.h>
#include <cstdint>
#include <mmu/AddressSpace.h>
#include <cpu/InterruptManager.h>

class Serial : public AddressSpace {
public:
    Serial(InterruptManager& int_manager) : int_manager_(int_manager) {}
    void tick() {
        if ((SC_ & SHIFT_CLOCK) && ongoing_transfer_ &&
            ++t_cycle_count_ % (CLOCK_FREQ / SERIAL_FREQ) == 0) {
            //Send bit 7 of SB_ on wire
            SB_ <<= 1;
            //TODO: Write received bit to bit 0 of SB_, using placeholder here
            SB_ |= 1;
            //Request Serial Interrupt after 1 byte is sent out
            if (t_cycle_count_ % ((CLOCK_FREQ / SERIAL_FREQ) * 8) == 0) {
                ongoing_transfer_ = false;
                t_cycle_count_ = 0;
                SB_ = 0xFF;
                //std::cout << "\nRequesting Serial Interrupt" << std::endl;
                int_manager_.request_interrupt(InterruptType::SERIAL);
            }
        }
    }
    uint8_t read(uint16_t addr) override {
        switch(addr) {
            case 0xFF01:
                return SB_;
            case 0xFF02:
                return SC_ | 0x7E;
            default:
                return 0xFF;
        }
    }
    void write(uint16_t addr, uint8_t val) override {
        switch(addr) {
            case 0xFF01:
                SB_ = val;
                //std::cout << static_cast<char>(val);
                break;
            case 0xFF02:
                SC_ = val;
                if (SC_ & TRANSFER_START) {
                    std::cout << static_cast<char>(SB_);
                    //LOG(static_cast<char>(SB_));
                    ongoing_transfer_ = true;
                    t_cycle_count_ = 0;
                } else {
                    ongoing_transfer_ = false;
                }
                break;
        }
    }

    bool manages(uint16_t addr) override {
        return addr == 0xFF01 || addr == 0xFF02;
    }
    
    static constexpr uint32_t SERIAL_FREQ{8192};
    static constexpr uint8_t  TRANSFER_START{1 << 7};
    static constexpr uint8_t  SHIFT_CLOCK{1};
    
private:
    InterruptManager& int_manager_;
    uint16_t t_cycle_count_{0};
    bool ongoing_transfer_{false};

    uint8_t SB_{0x00};
    uint8_t SC_{0x7E};
};