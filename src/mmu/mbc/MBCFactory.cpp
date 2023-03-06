#include <mmu/mbc/MBCFactory.h>

const std::unordered_map<uint8_t, std::shared_ptr<MBC> (&)(std::ifstream&)> MBCFactory::factories_ = {
    {0x00, MBCFactory::create_mbc<MBC0>},
    
    {0x01, MBCFactory::create_mbc<MBC1>},
    {0x02, MBCFactory::create_mbc<MBC1>},
    {0x03, MBCFactory::create_mbc<MBC1>},

    /*
    {0x05, MBCFactory::create_mbc<MBC2>},
    {0x06, MBCFactory::create_mbc<MBC2>},

    {0x0F, MBCFactory::create_mbc<MBC3>},
    {0x10, MBCFactory::create_mbc<MBC3>},
    {0x11, MBCFactory::create_mbc<MBC3>},
    {0x12, MBCFactory::create_mbc<MBC3>},
    {0x13, MBCFactory::create_mbc<MBC3>},
    */
};