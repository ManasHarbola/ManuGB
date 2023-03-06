#pragma once

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <Constants.h>
#include <Logging.h>
#include <mmu/AddressableArray.h>

class MBC : public AddressSpace {
public:
    virtual ~MBC() = default;
    virtual void tick() {}
};

template<size_t CART_SPACE, size_t RAM_SPACE>
class BasicMBC : public MBC {
public:
    BasicMBC(std::ifstream& rom_file) {
        if (rom_file.is_open()) {
            rom_file.clear();
            rom_file.seekg(0, std::ios::beg);
            rom_file.read((char*) cart_, CART_SPACE);

            std::cout << "HEADER INFO:" << std::endl
                      << "CART TYPE: " << print_num<int>(cart_[0x147], std::hex) << std::endl
                      << "ROM SIZE: " << print_num<int>(cart_[0x148], std::hex) << std::endl
                      << "RAM SIZE: " << print_num<int>(cart_[0x149], std::hex) << std::endl
                      << std::endl;

        } else {
            std::cout << "Error: ROM file not open" << std::endl;
        }   
    }

    virtual bool manages(uint16_t addr) override {
        return in_range_closed(addr, 0, 0x7FFF) || in_range_closed(addr, 0xA000, 0xBFFF);
    }
protected:
    uint8_t cart_[CART_SPACE];
    uint8_t ram_[RAM_SPACE];
};

class MBC0 : public BasicMBC<32_kb, 8_kb> {
public:
    MBC0(std::ifstream& rom_file) : BasicMBC(rom_file) {}
    virtual uint8_t read(uint16_t addr) override {
        if (in_range_closed(addr, 0, 0x7FFF))
            return cart_[addr];
        else if (in_range_closed(addr, 0xA000, 0xBFFF))
            return ram_[addr - 0xA000];
        
        return 0xFF;
    }
    virtual void write(uint16_t addr, uint8_t val) override {
        if (in_range_closed(addr, 0xA000, 0xBFFF))
            ram_[addr - 0xA000] = val;
    }
    
};

/*
Note to self:
Using the cartridge header, we can enable/disable MBC-specific components
specific to the rom (e.g. some roms use a MBC3 + timer, some just use MBC3)
*/

class MBC1 : public BasicMBC<2_mb, 32_kb> {
public:
    MBC1(std::ifstream& rom_file) : BasicMBC(rom_file), 
    rom_size_(32 * (1 << static_cast<uint32_t>(cart_[0x148]))),
    rom_banks_(1 << (static_cast<uint32_t>(cart_[0x148] + 1))) {
        switch(cart_[0x149]) {
            case 0:
                ram_size_ = 0;
                //ram_banks_ = 0;
                break;
            case 2:
                ram_size_ = 8;
                //ram_banks_ = 1;
                break;
            case 3:
                ram_size_ = 32;
                //ram_banks_ = 4
                break;
            case 4:
                ram_size_ = 128;
                //ram_banks_ = 16;
                break;
            case 5:
                ram_size_ = 64;
                //ram_banks_ = 8;
                break;
            default:
                ram_size_ = 0;
                break;
        }
    }
    virtual uint8_t read(uint16_t addr) override {
        if (addr < 0x4000) {
            addr &= 0x3FFF;
            return bank_mode_ ? cart_[(register2_ << 19) | static_cast<uint32_t>(addr)] : cart_[addr];
        } else if (addr < 0x8000) {
            addr &= 0x3FFF;
            return cart_[(register2_ << 19) | (rom_bank1_ << 14) | static_cast<uint32_t>(addr)];
        } else if (addr >= 0xA000 && addr < 0xC000 && ram_enable_) {
            addr &= 0x1FFF;
            return bank_mode_ ? ram_[(register2_ << 13) | static_cast<uint32_t>(addr)] : ram_[addr];
        }
        return 0xFF;        
    }
    virtual void write(uint16_t addr, uint8_t val) override {
        if (addr < 0x2000) {
            ram_enable_ = ((val & 0x0A) == 0x0A);
        } else if (addr < 0x4000) {
            rom_bank1_ = val & 0x1F;
            if (rom_bank1_ == 0) {
                rom_bank1_ = 1;
            } else if (rom_banks_ < 32 && rom_bank1_ >= rom_banks_) {
                rom_bank1_ &= (rom_banks_ - 1);
            }
        } else if (addr < 0x6000) {
            if ((ram_size_ == 32 || rom_size_ >= 1024)) {
                register2_ = static_cast<uint32_t>(val & 3);
            }
        } else if (addr < 0x8000) {
            if ((ram_size_ > 8 || rom_size_ > 512)) {
                bank_mode_ = (val & 1);
            }
        } else if (addr >= 0xA000 && addr < 0xC000 && ram_enable_) {
            addr &= 0x1FFF;
            if (bank_mode_) {
                ram_[(register2_ << 13) | static_cast<uint32_t>(addr)] = val;
            } else {
                ram_[addr] = val;
            }
        }
    }


private:
    //sizes are listed in kb
    const uint32_t rom_size_;
    uint32_t ram_size_;

    //uint32_t rom_bank0_{0};
    uint32_t rom_bank1_{1};
    const uint32_t rom_banks_;
    uint32_t register2_{0};

    bool ram_enable_{false};
    bool bank_mode_{false};

};

class MBC2 : public BasicMBC<256_kb, 512> {
public:

};

class MBC3 : public BasicMBC<2_mb, 64_kb> {
public:

};