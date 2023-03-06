#include "MMU.h"
#include <algorithm>
#include <initializer_list>

MMU::MMU(InterruptManager& int_manager, Timer& timer, Serial& port,
         PPU& ppu, Joypad& buttons) :
int_manager_(int_manager), timer_(timer), port_(port),
ppu_(ppu), buttons_(buttons) {}

bool MMU::load_rom(const std::string& rom_path) {
    //TODO: Update this to accomdate loading of ALL roms,
    //not just 32kb test roms that don't need MBC
    std::ifstream rom_file(rom_path.c_str(), std::ios::in | std::ios::binary);
    if (!rom_file) {
        std::cout << "file not opened..." << std::endl;
        return false;
    }

    rom_file.seekg(0, std::ios::end);
    size_t sz = rom_file.tellg();
    std::cout << "Size was: " << sz << std::endl;
    if (sz == -1) {
        std::cout << "Something happened..." << std::endl;
        return false;
    }
    
    uint8_t cart_header[0x150];
    rom_file.seekg(0, std::ios::beg);
    //rom_file.read((char*) cart_, std::min(CART_SPACE, sz));
    
    //read the cartridge header to create mbc instance
    rom_file.read((char*) cart_header, 0x150);

    rom_file.seekg(0, std::ios::beg);

    //create mbc_ instance
    mbc_ = MBCFactory::create_mbc(cart_header[0x147], rom_file);

    if (!mbc_) {
        std::cout << "unable to create memory bank controller..." << std::endl;
        return false;
    }

    rom_file.close();
    
    /*
    //copy first 32k of rom onto mem_
    //std::copy(cart_, cart_ + 32_kb, rom_);
    for (size_t i = 0; i < 32_kb; i++) {
        rom_.write(i, cart_[i]);
    }
    */

    //set initial values of hardware registers
    //TODO: clean this up lol
    write(0xFF00, 0xCF);	
    write(0xFF01, 0x00);	
    write(0xFF02, 0x7E);	
    write(0xFF03, 0xFF);
    //write(0xFF04, 0xAB);	
    write(0xFF05, 0x00);	
    write(0xFF06, 0x00);	
    write(0xFF07, 0xF8);	
    write(0xFF0F, 0xE1);

    write(0xFF10, 0x80);	
    write(0xFF11, 0xBF);	
    write(0xFF12, 0xF3);	
    write(0xFF13, 0xFF);	
    write(0xFF14, 0xBF);	
    write(0xFF16, 0x3F);	
    write(0xFF17, 0x00);	
    write(0xFF18, 0xFF);	
    write(0xFF19, 0xBF);	
    write(0xFF1A, 0x7F);	
    write(0xFF1B, 0xFF);	
    write(0xFF1C, 0x9F);	
    write(0xFF1D, 0xFF);	
    write(0xFF1E, 0xBF);	
    write(0xFF20, 0xFF);	
    write(0xFF21, 0x00);	
    write(0xFF22, 0x00);	
    write(0xFF23, 0xBF);	
    write(0xFF24, 0x77);	
    write(0xFF25, 0xF3);	
    write(0xFF26, 0xF1);	
    write(0xFF40, 0x91);	
    write(0xFF41, 0x85);	
    write(0xFF42, 0x00);	
    write(0xFF43, 0x00);	
    write(0xFF44, 0x00);	
    write(0xFF45, 0x00);	
    //write(0xFF46, 0xFF);	
    write(0xFF47, 0xFC);	
    write(0xFF4A, 0x00);	
    write(0xFF4B, 0x00);	
    write(0xFF4D, 0xFF);	
    write(0xFF4F, 0xFF);	
    write(0xFF51, 0xFF);	
    write(0xFF52, 0xFF);	
    write(0xFF53, 0xFF);	
    write(0xFF54, 0xFF);	
    write(0xFF55, 0xFF);	
    write(0xFF56, 0xFF);	
    write(0xFF68, 0xFF);	
    write(0xFF69, 0xFF);	
    write(0xFF6A, 0xFF);	
    write(0xFF6B, 0xFF);	
    write(0xFF70, 0xFF);	
    write(0xFFFF, 0x00);
    return true;
}

void MMU::tick() {
    //reset_joypad();
    if (mbc_)
        mbc_->tick();
    if (dma_transfer_lock_ > 0) {
        dma_transfer_lock_--;
        if (dma_transfer_lock_ == 0) {
            //dma pretends all of WRAM is mirrored
            if (DMA_ > 0xDF)
                DMA_ -= 0x20;
            uint16_t start = ((uint16_t) DMA_) << 8;
            for (uint16_t offset = 0; offset < 0xA0; offset++) {
                //write(0xFE00 | offset, read(start | offset));
                uint8_t v = read(start | offset);
                write(0xFE00 | offset, v);
            }
        }
    }
}

uint8_t MMU::read(uint16_t addr) {
    switch(addr) {
        case 0xFF46:
            return DMA_;
        case 0xFF0F:
            return int_manager_.get_IF();
        case 0xFFFF:
            return int_manager_.get_IE();
    }
    if (mbc_->manages(addr))
        return mbc_->read(addr);
    for (AddressSpace* component : read_list_) {
        if (component->manages(addr))
            return component->read(addr);
    }
    if (ppu_.manages(addr) &&
        !(addr >= 0xFE00 && addr < 0xFEA0 && dma_transfer())) {
        return ppu_.read(addr);
    }
    if (wram_.manages(addr) || (addr >= 0xE000 && addr < 0xFE00))
        return (addr >= 0xE000 && addr < 0xFE00) ? wram_.read(addr - 0xE000) : wram_.read(addr);
    if (addr >= 0xFEA0 && addr < 0xFF00)
        return 0x00;
    
    return 0xFF;
}

void MMU::write(uint16_t addr, uint8_t val) {
    switch(addr) {
        case 0xFF0F:
            //log for debugging
            //LOG("IF set to: " << (unsigned int) val);
            int_manager_.set_IF(val);
            break;
        case 0xFF46:
            DMA_ = val;
            dma_transfer_lock_ = DMA_TRANSFER_PERIOD;
            break;
        // Interrupt Enable
        case 0xFFFF:
            //log for debugging
            //LOG("IE set to: " << (unsigned int) val);
            int_manager_.set_IE(val);
            break;
        }
    if (mbc_->manages(addr))
        return mbc_->write(addr, val);
    for (AddressSpace* component : write_list_) {
        if (component->manages(addr))
            return component->write(addr, val);
    }
    if (ppu_.manages(addr) &&
        !(addr >= 0xFE00 && addr < 0xFEA0 && dma_transfer())) {
        return ppu_.write(addr, val);
    }
    if (wram_.manages(addr) || (addr >= 0xE000 && addr < 0xFE00))
        return (addr >= 0xE000 && addr < 0xFE00) ? wram_.write(addr - 0xE000, val) : wram_.write(addr, val);
}