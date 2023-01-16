#pragma once

#include <cstdint>

struct RGBA32 {
    constexpr RGBA32(uint8_t red=0, uint8_t green=0, 
           uint8_t blue=0, uint8_t alpha=0)
           : r(red), g(green), b(blue), a(alpha) {}
    uint8_t r, g, b, a;
};