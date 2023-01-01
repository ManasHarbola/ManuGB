#pragma once

#include <cstdint>
#include <Color.h>

constexpr uint32_t CLOCK_FREQ{4194304};
constexpr uint32_t CYCLES_PER_FRAME{70224};
constexpr uint32_t DISPLAY_WIDTH{160};
constexpr uint32_t DISPLAY_HEIGHT{144}; 

constexpr size_t operator""_kb(unsigned long long num){return (1 << 10) * num;}
constexpr size_t operator""_mb(unsigned long long num){return (1 << 20) * num;}


constexpr size_t CART_SPACE{2_mb};
constexpr size_t ROM_BANK_SPACE{32_kb};
constexpr size_t WRAM_BANK_SPACE{8_kb};
constexpr size_t VRAM_SPACE{8_kb};
constexpr size_t OAM_SPACE{0x100};
constexpr size_t EXTERNAL_RAM_SPACE{8_kb};

//maps 2-bit color number to RGBA color
/*
constexpr uint32_t LIGHTEST{0x9BBC0FFF}, LIGHT{0x8BAC0FFF},
                   DARK{0x306230FF}, DARKEST{0x0F380FFF};
*/
/*
constexpr uint32_t LIGHTEST{0xFAFBF6FF}, LIGHT{0xC6B7BEFF},
                   DARK{0x565A75FF}, DARKEST{0x0F0F1BFF};
*/

const RGBA32 LIGHTEST(0xFA, 0xFB, 0xF6, 0xFF), LIGHT(0xC6, 0xB7, 0xBE, 0xFF),
             DARK(0x56, 0x5A, 0x75, 0xFF), DARKEST(0x0F, 0x0F, 0x1B, 0xFF);

const RGBA32 COLOR_MAP[]{LIGHTEST, LIGHT, DARK, DARKEST};