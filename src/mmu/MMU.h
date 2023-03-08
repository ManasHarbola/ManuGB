#pragma once

#include <Logging.h>
#include <mmu/mbc/MBC.h>
#include <mmu/mbc/MBCFactory.h>
#include <mmu/AddressSpace.h>
#include <mmu/AddressableArray.h>
#include <cpu/InterruptManager.h>
#include <timer/Timer.h>
#include <serial/Serial.h>
#include <joypad/Joypad.h>
#include <ppu/PPU.h>
#include <joypad/Joypad.h>
#include <Constants.h>
#include <string>
#include <array>
#include <memory>
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

class MMU : public AddressSpace {
    public:
        MMU(InterruptManager& int_manager, Timer& timer, Serial& port,
            PPU& ppu, Joypad& buttons);

        void tick();
        bool load_rom(const std::string &rom_path);
        uint8_t read(uint16_t addr) override;
        void write(uint16_t addr, uint8_t val) override;
        bool manages(uint16_t addr) override {return true;}

        static constexpr size_t DMA_TRANSFER_PERIOD{644};
        static constexpr uint8_t SELECT_ACTION{1 << 5};
        static constexpr uint8_t SELECT_DIRECTION{1 << 4};
        //returns whether a OAM DMA transfer is currently happening
        bool dma_transfer() {
            return dma_transfer_lock_ > 0;
        }

    private:
        //uint8_t curr_bank_{1};
        //uint8_t cart_[CART_SPACE];
        std::shared_ptr<MBC> mbc_{nullptr};

        //AddressableArray<0x8000> rom_{0, 0x7FFF};
        AddressableArray<0x2000> wram_{0xC000, 0xDFFF};
        //AddressableArray<0x2000> external_ram_{0xA000, 0xBFFF};

        //Sound Registers (Sound not implemented yet)
        AddressableArray<0x17> nr_{0xFF10, 0xFF26};
        //ONLY the HRAM is accessible to the CPU during DMA transfer
        AddressableArray<0x80> hram_{0xFF80, 0xFFFF};
        //0xFF46: DMA
        uint8_t DMA_{0xFF};
        //indicates whether a dma_transfer is in process
        uint16_t dma_transfer_lock_{0};

        InterruptManager& int_manager_;
        Timer& timer_;
        Serial& port_;
        PPU& ppu_;
        Joypad& buttons_;

        //for iterating through components in read/writes
        AddressSpace* const read_list_[5]{/*&rom_, &external_ram_, */ &hram_, &nr_,
                                             &buttons_, &port_, &timer_};
        AddressSpace* const write_list_[5]{/*&external_ram_,*/ &hram_, &buttons_,
                                          &nr_, &port_, &timer_};
};