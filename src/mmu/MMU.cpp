#include "MMU.h"

MMU::MMU(InterruptManager& int_manager, Timer& timer, PPU& ppu) :
int_manager_(int_manager), timer_(timer), ppu_(ppu) {
    //clear rom_ and cart_
    memset((void *) &mem_[0], 0, sizeof(mem_));
    memset((void *) &cart_[0], 0, sizeof(cart_));
}

bool MMU::load_rom(const std::string &rom_path) {
    //TODO: Update this to accomdate loading of ALL roms,
    //not just 32kb test roms that don't need MBC
    std::ifstream rom_file(rom_path.c_str(), std::ios::in | std::ios::binary);
    if (!rom_file) {
        std::cout << "file not opened..." << std::endl;
    }
    rom_file.seekg(0, std::ios::end);
    std::streamsize sz = rom_file.tellg();
    std::cout << "Size was: " << sz << std::endl;
    if (sz == -1) {
        std::cout << "Something happened..." << std::endl;
        return false;
    }
    rom_file.seekg(0, std::ios::beg);
    rom_file.read((char*) &cart_[0], min(sizeof(cart_), sz));
    rom_file.close();

    //copy first 32k of rom onto mem_
    memcpy((void *) &mem_[0], (void *) &cart_[0], 0x8000);

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

uint8_t MMU::read(uint16_t addr) {
    //return mem_[addr];
    //TODO: MUST ADAPT FOR memory bank switching the future to accomodate >32kb roms
    if (addr >= 0x0000 && addr <= 0x7FFF)
        return mem_[addr];

    switch(addr) {
        case 0xFF04:
        case 0xFF05:
        case 0xFF06:
        case 0xFF07:
            return timer_.read(addr);
        //LY hardcode
        case 0xFF44:
            return 0x90;
        // interruptions
        case 0xFFFF:
            return int_manager_.get_IE();
        case 0xFF0F:
            return int_manager_.get_IF();
        default:
            return mem_[addr];
    }

}

void MMU::write(uint16_t addr, uint8_t val) {
    //this is ROM - don't care about bank switching right now
    //TODO: add bank switching in response to writes
    if (addr >= 0x0000 && addr <= 0x7FFF)
        return;
    
    //handle echo ram in a crude manner right now
    //TODO: fix this
    if (addr >= 0xE000 && addr <= 0xFDFF) {
        mem_[addr] = val;
        mem_[addr - 0x2000] = val;
        return;
    }

    switch(addr) {
        case 0xFF04:
        case 0xFF05:
        case 0xFF06:
        case 0xFF07:
            timer_.write(addr, val);
            break;
        // IE
        case 0xFFFF:
            LOG("IE set to: " << (unsigned int) val);
            int_manager_.set_IE(val);
            break;
        // IF
        case 0xFF0F:
            LOG("IF set to: " << (unsigned int) val);
            int_manager_.set_IF(val);
            break;
        default:
            mem_[addr] = val;
            break;
    }

    //for testing cpu
    if (addr == 0xff01) {
        std::cout << (char) mem_[addr];
    }
}