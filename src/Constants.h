#pragma once

#include <cstdint>
#include <Color.h>

constexpr uint32_t CLOCK_FREQ{4194304};
constexpr uint32_t CYCLES_PER_FRAME{70224};
constexpr uint32_t DISPLAY_WIDTH{160};
constexpr uint32_t DISPLAY_HEIGHT{144}; 

constexpr size_t operator""_kb(unsigned long long num){return (1 << 10) * num;}
constexpr size_t operator""_mb(unsigned long long num){return (1 << 20) * num;}

//returns if val is in interval [left_bound, right_bound]
template<typename Tv, typename Tl, typename Tr>
constexpr inline bool in_range_closed(const Tv val, const Tl left_bound, const Tr right_bound){
    return (val >= left_bound) && (val <= right_bound);
}
//return if val is in interval [left_bound, right_bound)
template<typename Tv, typename Tl, typename Tr>
constexpr inline bool in_range(const Tv val, const Tl left, const Tr right) {
    return (val >= left) && (val < right);
}


constexpr size_t CART_SPACE{2_mb};
constexpr size_t ROM_BANK_SPACE{32_kb};
constexpr size_t WRAM_BANK_SPACE{8_kb};
constexpr size_t VRAM_SPACE{8_kb};
constexpr size_t OAM_SPACE{0x100};
constexpr size_t EXTERNAL_RAM_SPACE{8_kb};

//maps 2-bit color number to RGBA color

//Green Palette
constexpr RGBA32 LIGHTEST(0xE0, 0xF8, 0xD0, 0xFF), LIGHT(0x88, 0xC0, 0x70, 0xFF),
                 DARK(0x34, 0x68, 0x56, 0xFF), DARKEST(0x08, 0x18, 0x20, 0xFF);

//Black and White Palette
/*
constexpr RGBA32 LIGHTEST(0xFA, 0xFB, 0xF6, 0xFF), LIGHT(0xC6, 0xB7, 0xBE, 0xFF),
                 DARK(0x56, 0x5A, 0x75, 0xFF), DARKEST(0x0F, 0x0F, 0x1B, 0xFF);
*/

constexpr RGBA32 COLOR_MAP[]{LIGHTEST, LIGHT, DARK, DARKEST};