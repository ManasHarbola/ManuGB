#pragma once

#include <mmu/AddressSpace.h>
#include <vector>

template<size_t N>
class AddressableArray : public AddressSpace {
public:
    AddressableArray(uint16_t start, uint16_t end) : start_(start), end_(end) {}
    uint8_t read(uint16_t addr) override {
        return mem_[addr - start_];
    }
    void write(uint16_t addr, uint8_t val) override {
        mem_[addr - start_] = val;
    }
    bool manages(uint16_t addr) override {
        return addr >= start_ && addr <= end_;
    }
private:
    std::array<uint8_t, N> mem_;
    uint16_t start_, end_;
};
