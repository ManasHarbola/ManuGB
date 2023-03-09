#pragma once

#include <fstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdint>
#include <Constants.h>
#include <Logging.h>
#include <mmu/AddressableArray.h>

class MBC : public AddressSpace {
public:
    virtual ~MBC() = default;
    virtual bool rom_loaded() const = 0;
    virtual bool save_state(std::ofstream& save_file) = 0;
    virtual bool load_save(std::ifstream& save_file) = 0;
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
    
    bool save_state(std::ofstream& save_file) override {
        if (save_file.good()) {
            save_file.write(reinterpret_cast<const char*>(ram_), RAM_SPACE * sizeof(uint8_t));
            save_file.flush();
        } else {
            std::cout << "Cannot save game state: bad save file handle" << std::endl;
        }
        return save_file.good();
    }

    bool load_save(std::ifstream& save_file) override {
        if (save_file.good()) {
            save_file.clear();
            save_file.seekg(0, std::ios::end);
            size_t sz = save_file.tellg();
            save_file.seekg(0, std::ios::beg);
            save_file.read(reinterpret_cast<char*>(ram_), std::min(RAM_SPACE * sizeof(uint8_t), sz));
        } else {
            std::cout << "Cannot load save state: bad save file handle" << std::endl;
        }
        return save_file.good();
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
    virtual uint8_t read(uint16_t addr) override;
    virtual void write(uint16_t addr, uint8_t val) override;
private:
    uint32_t rom_bank1_{1};
    uint32_t register2_{0};
    bool ram_enable_{false};
    bool bank_mode_{false};
};

class MBC2 : public BasicMBC<256_kb, 512> {
public:

};
class MBC3 : public BasicMBC<4_mb, 64_kb> {
public:
    MBC3(std::ifstream& rom_file) : BasicMBC(rom_file) {}
    virtual uint8_t read(uint16_t addr) override;
    virtual void write(uint16_t addr, uint8_t val) override;
    virtual void tick() override;
private:
    uint32_t t_cycle_lock_{0};
    uint8_t rtc_[5]{0};
    uint8_t current_rtc_[5]{0};
    const static uint16_t RTC_RANGES[4];
    uint32_t rom_bank1_{1}; 
    uint8_t ram_bank_rtc_register_{0};
    bool ram_timer_enable_{false};
    //for handling 00h->01h latch clock data
    bool latch_queued{false};
};