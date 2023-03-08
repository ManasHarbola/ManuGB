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
    virtual bool rom_loaded() const = 0;
    virtual void tick() {}
};

template<size_t CART_SPACE, size_t RAM_SPACE>
class BasicMBC : public MBC {
public:
    BasicMBC(std::ifstream& rom_file) {
        if (!rom_file.is_open()) {
            std::cout << "Error: ROM file not open" << std::endl;
            return;
        }

        rom_file.clear();
        rom_file.seekg(0, std::ios::beg);
        rom_file.read((char*) cart_, CART_SPACE);
        rom_size_ = (1 << static_cast<uint32_t>(cart_[0x148])) << 5;
        if (in_range_closed(cart_[0x149], 2, 4)) {
            ram_size_ = 1 << ((cart_[0x149] << 1) - 1);
        } else if (cart_[0x149] == 5) {
            ram_size_ = 64;
        } else {
            ram_size_ = 0;
        }
        std::cout << "HEADER INFO:" << std::endl
                    << "CART TYPE: " << print_num<int>(cart_[0x147], std::hex) << std::endl
                    << "ROM SIZE: " << print_num<int>(cart_[0x148], std::hex) << std::endl
                    << "RAM SIZE: " << print_num<int>(cart_[0x149], std::hex) << std::endl
                    << std::endl;
        rom_loaded_ = true;
    }
    virtual bool manages(uint16_t addr) override {
        return in_range_closed(addr, 0, 0x7FFF) || in_range_closed(addr, 0xA000, 0xBFFF);
    }
    virtual bool rom_loaded() const override {return rom_loaded_;}
    inline const uint32_t rom_size_kb() const {return rom_size_;}
    inline const uint32_t ram_size_kb() const {return ram_size_;}
    inline const uint32_t num_rom_banks() const {return rom_size_ >> 4;}

protected:
    uint8_t cart_[CART_SPACE];
    uint8_t ram_[RAM_SPACE];
        
private:
    //status flag for whether MBC loaded rom correctly
    bool rom_loaded_{false};
    //size reported in KBytes 
    uint32_t rom_size_;
    uint32_t ram_size_;
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
    MBC1(std::ifstream& rom_file) : BasicMBC(rom_file) {}
    virtual uint8_t read(uint16_t addr) override {
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
    virtual void write(uint16_t addr, uint8_t val) override {
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
private:
    uint32_t rom_bank1_{1};
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