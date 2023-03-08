#pragma once

#include <cstdint>

class AddressSpace {
public:
    virtual ~AddressSpace() = default;
    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t val) = 0;
    virtual bool manages(uint16_t addr) = 0;
};