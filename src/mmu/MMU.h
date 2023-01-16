#pragma once

#include <Logging.h>
#include <cpu/InterruptManager.h>
#include <timer/Timer.h>
#include <serial/Serial.h>
#include <ppu/PPU.h>
#include <Constants.h>
#include <string>
#include <string.h>
#include <cstring>
#include <fstream>

enum class JoyPad : uint8_t {
    A_BUTTON = 0,
    RIGHT = 1,
    B_BUTTON = 2,
    LEFT = 3,
    SELECT = 4,
    UP = 5,
    START = 6,
    DOWN = 7
};

class MMU {
    public:
        MMU(InterruptManager& int_manager, Timer& timer, Serial& port, PPU& ppu);
        void tick();
        bool load_rom(const std::string &rom_path);
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t val);
        void reset_joypad() {P1_ |= 0x0F;}
        void key_press(JoyPad key) {
            switch(key) {
                case JoyPad::RIGHT:
                case JoyPad::LEFT:
                case JoyPad::UP:
                case JoyPad::DOWN:
                    if (!((bool)(P1_ & SELECT_DIRECTION)))
                        P1_ &= ~(1 << (((uint8_t) key) >> 1));
                    break;
                case JoyPad::A_BUTTON:
                case JoyPad::B_BUTTON:
                case JoyPad::SELECT:
                case JoyPad::START:
                    if (!((bool)(P1_ & SELECT_ACTION)))
                        P1_ &= ~(1 << (((uint8_t) key) >> 1));
                    break;
            }
        }
        
        static constexpr size_t DMA_TRANSFER_PERIOD{644};
        static constexpr uint8_t SELECT_ACTION{1 << 5};
        static constexpr uint8_t SELECT_DIRECTION{1 << 4};
        //returns whether a OAM DMA transfer is currently happening
        bool dma_transfer() {return dma_transfer_lock_ <= DMA_TRANSFER_PERIOD - 4;}

    private:
        //uint8_t curr_bank_{1};
        uint8_t cart_[CART_SPACE];

        uint8_t rom_[ROM_BANK_SPACE]{0};
        uint8_t wram_[WRAM_BANK_SPACE]{0};
        uint8_t external_ram_[EXTERNAL_RAM_SPACE]{0};
        
        //Sound Registers (Sound not implemented yet)
        uint8_t nr_[0x17];
        
        //ONLY the HRAM is accessible to the CPU during DMA transfer
        uint8_t hram_[127];

        //0xFF00: P1/JOYP Register
        uint8_t P1_{0xCF};

        /*
        //OxFF01: Serial Transfer
        uint8_t SB_{0x00};
        //0xFF02: Serial Control
        uint8_t SC_{0x7E};
        */

        //0xFF46: DMA
        uint8_t DMA_{0xFF};

        //indicates whether a dma_transfer is in process
        uint16_t dma_transfer_lock_{0};


        InterruptManager& int_manager_;
        Timer& timer_;
        Serial& port_;
        PPU& ppu_;
};