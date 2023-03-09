#include "MBC.h"

uint8_t MBC1::read(uint16_t addr) {
    if (addr < 0x4000) {
        addr &= 0x3FFF;
        return bank_mode_ ? cart_[((register2_ << 19) | static_cast<uint32_t>(addr)) % (rom_size_kb() << 10)] : cart_[addr];
    } else if (addr < 0x8000) {
        return cart_[((register2_ << 19) | (rom_bank1_ << 14) | static_cast<uint32_t>(addr & 0x3FFF)) % (rom_size_kb() << 10)];
    } else if (addr >= 0xA000 && addr < 0xC000) {
        if (ram_enable_) {
            addr &= 0x1FFF;
            return bank_mode_ ? ram_[((register2_ << 13) | static_cast<uint32_t>(addr)) % (ram_size_kb() << 10)] : ram_[addr];
        } else {
            #ifdef PROFILE
            std::cout << "Ram disabled, cannot access: " << print_num<int>(addr) << std::endl;
            #endif
        }
    } else {
        #ifdef PROFILE
        std::cout << "Cannot read from location " << print_num<int>(addr, std::hex) << std::endl;
        #endif
    }
    return 0xFF;        
}
void MBC1::write(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) {
        if (rom_size_kb() != 0)
            ram_enable_ = ((val & 0x0F) == 0x0A);
    } else if (addr < 0x4000) {
        rom_bank1_ = val & 0x1F;
        if (rom_bank1_ == 0) {
            rom_bank1_ = 1;
        } else if (rom_bank1_ >= num_rom_banks()) {
            rom_bank1_ &= (num_rom_banks() - 1);
        }
    } else if (addr < 0x6000) {
        if ((ram_size_kb() == 32 || rom_size_kb() >= 1024)) {
            register2_ = static_cast<uint32_t>(val & 3);
        }
    } else if (addr < 0x8000) {
        if ((ram_size_kb() > 8 || rom_size_kb() > 512)) {
            bank_mode_ = (val & 1) != 0;
        }
    } else if (addr >= 0xA000 && addr < 0xC000) {
        if (ram_enable_) {
            addr &= 0x1FFF;
            if (bank_mode_) {
                ram_[((register2_ << 13) | static_cast<uint32_t>(addr)) % (ram_size_kb() << 10)] = val;
            } else {
                ram_[addr] = val;
            }
        } else {
            #ifdef PROFILE
            std::cout << "Ram currently disabled, cannot access " << print_num<int>(addr) << std::endl;
            #endif
        }
    }
}

const uint16_t MBC3::RTC_RANGES[4]{60, 60, 24, 256};

uint8_t MBC3::read(uint16_t addr) {
    if (addr < 0x4000) {
        return cart_[addr];
    } else if (addr < 0x8000) {
        return cart_[((rom_bank1_ << 14) | static_cast<uint32_t>(addr & 0x3FFF)) % (rom_size_kb() << 10)];
    } else if (in_range(addr, 0xA000, 0xC000) /*&& ram_timer_enable_*/) {
        if (ram_bank_rtc_register_ < 4) {
            //ram
            uint32_t new_addr = ((ram_bank_rtc_register_ << 13) | static_cast<uint32_t>(addr & 0x1FFF)) % (ram_size_kb() << 10);
            return ram_[new_addr];
        } else if (in_range_closed(ram_bank_rtc_register_, 0x08, 0x0C)) {
            //rtc
            return rtc_[ram_bank_rtc_register_ - 0x08];
        }
    }
    return 0xFF;
}

void MBC3::write(uint16_t addr, uint8_t val) {
    if (in_range(addr, 0, 0x2000)) {
        val &= 0xF;
        if (val == 0xA)
            ram_timer_enable_ = true;
        else if (val == 0)
            ram_timer_enable_ = false;
    } else if (in_range(addr, 0x2000, 0x4000)) {
        //rom_bank1_ = val & 0x7F;
        rom_bank1_ = (rom_size_kb() <= 2048) ? val & 0x7F : val;
        //idk if this is right
        if (rom_bank1_ == 0)
            rom_bank1_ = 1;
    } else if (in_range(addr, 0x4000, 0x6000)) {
        ram_bank_rtc_register_ = val;
    } else if (in_range(addr, 0x6000, 0x8000)) {
        if (!latch_queued) {
            latch_queued = (val == 0);
        } else if (val == 1) {
            //copy current_rtc_ over to rtc_
            std::copy(std::begin(current_rtc_), std::end(current_rtc_), std::begin(rtc_));
            latch_queued = false;
        } else {
            //cancel the latch?
            latch_queued = false;
        }
    } else if (in_range(addr, 0xA000, 0xC000) /*&& ram_timer_enable_*/) {
        if (ram_bank_rtc_register_ < 4) {
            //ram
            uint32_t new_addr = ((ram_bank_rtc_register_ << 13) | static_cast<uint32_t>(addr & 0x1FFF)) % (ram_size_kb() << 10);
            ram_[new_addr] = val;
        } else if (in_range_closed(ram_bank_rtc_register_, 0x08, 0x0C)) {
            //rtc
            rtc_[ram_bank_rtc_register_ - 0x08] = val;
        }
    }
}

//necessary for ticking the current_rtc_
void MBC3::tick() {
    //check if a second has elapsed
    if (++t_cycle_lock_ == CLOCK_FREQ) {
        t_cycle_lock_ = 0;
        bool carry = true;
        for (int i = 0; i < 4 && carry; i++) {
            current_rtc_[i] = static_cast<uint8_t>((static_cast<uint16_t>(current_rtc_[i]) + 1) % RTC_RANGES[i]);
            carry = (current_rtc_[i] == 0);
        }
        if (carry) {
            //flip upper bit of day counter
            current_rtc_[4] ^= 1;
            //set day counter carry bit to signal overflow
            current_rtc_[4] |= 0x80;
        }
    }
}