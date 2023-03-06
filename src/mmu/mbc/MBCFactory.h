#pragma once

#include <mmu/mbc/MBC.h>
#include <memory>
#include <fstream>
#include <unordered_map>

#include <Logging.h>

class MBCFactory {
public:
    MBCFactory() = delete;
    static std::shared_ptr<MBC> create_mbc(uint8_t cart_type, std::ifstream& rom_file) {
        std::cout << print_num<int>(cart_type, std::hex) << std::endl;
        return factories_.at(cart_type)(rom_file);
    }
private:
    template<typename MBCType>
    static std::shared_ptr<MBC> create_mbc(std::ifstream& rom_file) {
        return std::make_shared<MBCType>(rom_file);
    }
    static const std::unordered_map<uint8_t, std::shared_ptr<MBC> (&)(std::ifstream&)> factories_;
};