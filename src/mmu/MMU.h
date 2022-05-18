#pragma once

class MMU {
    public:
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t val);
    private:
};