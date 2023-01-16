#include "MMU.h"
#include <algorithm>

MMU::MMU(InterruptManager& int_manager, Timer& timer, Serial& port, PPU& ppu) :
int_manager_(int_manager), timer_(timer), port_(port), ppu_(ppu) {}

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
    rom_file.seekg(0, std::ios::beg);
    rom_file.read((char*) cart_, std::min(CART_SPACE, sz));
    rom_file.close();

    //copy first 32k of rom onto mem_
    std::copy(cart_, cart_ + 32_kb, rom_);

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
    write(0xFF46, 0xFF);	
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
    reset_joypad();
    if (dma_transfer_lock_ > 0) {
        dma_transfer_lock_--;
        if (dma_transfer_lock_ == 0) {
            //dma pretends all of WRAM is mirrored
            if (DMA_ > 0xDF)
                DMA_ -= 0x20;
            uint16_t start = ((uint16_t) DMA_) << 8;
            for (uint16_t offset = 0; offset < 0xA0; offset++)
                write(0xFE00 | offset, read(start | offset));
        }
    }
}

uint8_t MMU::read(uint16_t addr) {
    //TODO: MUST ADAPT FOR memory bank switching the future to accomodate >32kb roms
    //ROM BANKS 00, NN
    if (addr < 0x8000)
        return rom_[addr];
    //VRAM access
    if (addr < 0xA000)
        return ppu_.read(addr);
    //External RAM Access
    if (addr < 0xC000)
        return external_ram_[addr - 0xA000];
    //Working RAM Access
    if (addr < 0xE000)
        return wram_[addr - 0xC000];
    //ECHO RAM Access (echo of wram bank 0)
    if (addr < 0xFE00)
        return wram_[addr - 0xE000];
    //OAM - only accessible by CPU/PPU when DMA is NOT happening
    if (addr < 0xFEA0 && !dma_transfer())
        return ppu_.read(addr);
    //FEA0-FEFF range (return 0x00 for DMG)
    if (addr < 0xFF00)
        return 0x00;

    //HRAM access
    /*
    TODO: This should be the only memory accessible by CPU
          during DMA transfer
    */
    if (addr >= 0xFF80 && addr < 0xFFFF)
        return hram_[addr - 0xFF80];

    //I/O Register Access (0xFF00 - 0xFF7F range)
    //Additionally IE register at 0xFFFF
    switch(addr) {
        case 0xFF00:
            return P1_;
            //return 0xFF;
        
        /*
        case 0xFF01:
            return SB_;
        case 0xFF02:
            return SC_;
        */
        case 0xFF01:
        case 0xFF02:
            return port_.read(addr);

        //Timer Registers
        case 0xFF04:
        case 0xFF05:
        case 0xFF06:
        case 0xFF07:
            return timer_.read(addr);

        //Interrupt Flag
        case 0xFF0F:
            return int_manager_.get_IF();
        
        //All Sound (NRXX) registers omitted
        //TODO: Maybe implement sound?
        case 0xFF10 ... 0xFF26:
            return nr_[addr - 0xFF10];
        //DMA register
        case 0xFF46:
            return DMA_;

        //PPU/Graphics Registers
        // LCDC, STAT, SCY, SCX, LY, LYC
        case 0xFF40:
        case 0xFF41:
        case 0xFF42:
        case 0xFF43:
        case 0xFF44:
        case 0xFF45:
        // BGP, OBP0, OBP1, WY, WX
        case 0xFF47:
        case 0xFF48:
        case 0xFF49:
        case 0xFF4A:
        case 0xFF4B:
            return ppu_.read(addr);
        
        // Interrupt Enable
        case 0xFFFF:
            return int_manager_.get_IE();
        
    }
    
    //just return 0xFF
    return 0xFF;
}

void MMU::write(uint16_t addr, uint8_t val) {
    //!!!!!!!! READ BELOW
    //TODO: add bank switching in response to writes
    if (addr < 0x8000)
        return;
    //VRAM write
    //TODO: Block CPU access to VRAM when needed
    if (addr < 0xA000)
        return ppu_.write(addr, val);
    //External RAM Write
    if (addr < 0xC000) {
        external_ram_[addr - 0xA000] = val;
        return;
    }
    //Working RAM Write 
    if (addr < 0xE000) {
        wram_[addr - 0xC000] = val;
        return;
    }
    //ECHO RAM write
    if (addr < 0xFEA0) {
        wram_[addr - 0xE000] = val;
        return;
    }
    //OAM - write access only allowed when dma_transfer is not happening
    if (addr < 0xFEA0 && !dma_transfer())
        return ppu_.write(addr, val);
        
    //HRAM write
    /*
    TODO: This should be the only memory accessible by CPU
          during DMA transfer
    */
    if (addr >= 0xFF80 && addr < 0xFFFF) {
        hram_[addr - 0xFF80] = val;
    }
    
    //I/O Register Access (0xFF00 - 0xFF7F range)
    //Additionally IE register at 0xFFFF
    switch(addr) {
        case 0xFF00:
            P1_ = (val & 0xF0);
            break;
        /*
        case 0xFF01:
            SB_ = val;
            //for debugging/testing - print serial
            std::cout << (char) val;
            break;
        case 0xFF02:
            SC_ = val;
            break;
        */
        case 0xFF01:
        case 0xFF02:
            port_.write(addr, val);
            break;
            
        //Timer Registers
        case 0xFF04:
        case 0xFF05:
        case 0xFF06:
        case 0xFF07:
            return timer_.write(addr, val);
        //Interrupt Flag
        case 0xFF0F:
            //log for debugging
            LOG("IF set to: " << (unsigned int) val);
            int_manager_.set_IF(val);
            break;
        //All Sound (NRXX) registers omitted
        //TODO: Maybe implement sound?
        case 0xFF10 ... 0xFF26:
            nr_[addr - 0xFF10] = val;
            break;

        //DMA register
        //sets dma_transfer_lock_ to 644 t-cycles to block OAM r/w access
        case 0xFF46:
            DMA_ = val;
            dma_transfer_lock_ = DMA_TRANSFER_PERIOD;
            break;

        //PPU/Graphics Registers
        // LCDC, STAT, SCY, SCX, LY, LYC
        case 0xFF40:
        case 0xFF41:
        case 0xFF42:
        case 0xFF43:
        case 0xFF44:
        case 0xFF45:
        // BGP, OBP0, OBP1, WY, WX
        case 0xFF47:
        case 0xFF48:
        case 0xFF49:
        case 0xFF4A:
        case 0xFF4B:
            //TODO: CONTROL CPU ACCESS TO PPU REGISTERS APPROPRIATELY
            return ppu_.write(addr, val);
        
        // Interrupt Enable
        case 0xFFFF:
            //log for debugging
            LOG("IE set to: " << (unsigned int) val);
            int_manager_.set_IE(val);
            break;
    }
}