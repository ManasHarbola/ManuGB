#pragma once

#include <Logging.h>
#include <cpu/InterruptManager.h>
#include <Constants.h>
#include <Color.h>
#include <bitset>

enum class PPUState : uint8_t
{
    OAM_FETCH = 2,
    PIXEL_TRANSFER = 3,
    HBLANK = 0,
    VBLANK = 1
};
class PPU {
public:
    PPU(InterruptManager& int_manager);
    void tick();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t val);
    const char* get_buffer() {return frame_buffer_;}

    //PPU State Periods
    static constexpr uint16_t FETCH_PERIOD{80};
    static constexpr uint16_t TRANSFER_PERIOD{172};
    static constexpr uint16_t HBLANK_PERIOD{204};
    static constexpr uint16_t VBLANK_LINE_PERIOD{456};

    //OAM constants
    static constexpr uint8_t BG_WIN_OVER_OBJ{1 << 7};
    static constexpr uint8_t FLIP_SPRITE_Y{1 << 6};
    static constexpr uint8_t FLIP_SPRITE_X{1 << 5};
    static constexpr uint8_t SPRITE_PALETTE{1 << 4};
    static constexpr uint8_t OAM_ENTRIES{40};
    static constexpr uint8_t MAX_SPRITES_LINE{10};

    //LCDC Bitmasks
    static constexpr uint8_t LCD_ENABLE{1 << 7};
    static constexpr uint8_t WIN_TILE_MAP{1 << 6};
    static constexpr uint8_t WIN_ENABLE{1 << 5};
    static constexpr uint8_t BG_WIN_TILE_DATA{1 << 4};
    static constexpr uint8_t BG_TILE_MAP{1 << 3};
    static constexpr uint8_t OBJ_SIZE_FLAG{1 << 2};
    static constexpr uint8_t OBJ_ENABLE{1 << 1};
    static constexpr uint8_t BG_WIN_ENABLE{1};
    
    //STAT Bitmasks
    static constexpr uint8_t INTR_LYC{1 << 6};
    static constexpr uint8_t INTR_OAM{1 << 5};
    static constexpr uint8_t INTR_VBLANK{1 << 4};
    static constexpr uint8_t INTR_HBLANK{1 << 3};
    static constexpr uint8_t COINCIDENCE{1 << 2};
    static constexpr uint8_t STATE_MASK{0x3};


private:
    inline void inc_ly() {LY_ = (LY_ + 1) % 154;}
    inline bool ppu_enabled() {return LCDC_ & LCD_ENABLE;}
    inline PPUState get_ppu_state() {return (PPUState) (STAT_ & STATE_MASK);}
    inline bool vram_accessible() {
        return true;
        //return !ppu_enabled() || (get_ppu_state() != PPUState::PIXEL_TRANSFER);
    }
    inline bool oam_accessible() {
        //return true;
        return !ppu_enabled() || (get_ppu_state() == PPUState::HBLANK)
                              || (get_ppu_state() == PPUState::VBLANK);
    }
    //when LCDC.7 is set to 0 from 1, LY is set 0
    void disable_ppu() {
        LY_ = 0;
        win_ly_internal = 0;
        t_cycle_lock_ = 0;
        STAT_ &= ~STATE_MASK;
    }
    void enable_ppu() {
        //set state to OAM_FETCH
        STAT_ |= (uint8_t) PPUState::OAM_FETCH;
        skip_frame_ = true;
    }
    
    inline void set_ppu_state(PPUState state) {STAT_ = (STAT_ & 0xFC) | ((uint8_t) state);}
    inline void update_coincidence_flag() {
        STAT_ = (LY_ == LYC_) ? (STAT_ | COINCIDENCE) : (STAT_ & ~COINCIDENCE);
    }
    void check_for_irq(bool trigger=false) {
        PPUState state = get_ppu_state();
        if ((state == PPUState::OAM_FETCH && (STAT_ & INTR_OAM)) ||
            (trigger && (STAT_ & INTR_OAM)) ||
            (state == PPUState::HBLANK && (STAT_ & INTR_HBLANK)) ||
            (state == PPUState::VBLANK && (STAT_ & INTR_VBLANK)) ||
            ((STAT_ & INTR_LYC) && (STAT_ & COINCIDENCE)))
        {
            if (!stat_irq_) {
                int_manager_.request_interrupt(InterruptType::LCDSTAT);
                stat_irq_ = true;
            }
        } else {
            stat_irq_ = false;
        }
    }

    void draw_scanline();
    void draw_bg_scanline();
    void draw_win_scanline();
    void draw_obj_scanline();

    InterruptManager& int_manager_;
    uint16_t t_cycle_lock_{0};
    uint32_t total_t_cycles_{0};

    //used for skipping rendering of first frame
    //if lcd has just been enabled
    bool skip_frame_{false};
    //used for checking for "rising edge" in STAT irq
    bool stat_irq_{false};
    
    //for interfacing with SDL2
    char frame_buffer_[DISPLAY_WIDTH * DISPLAY_HEIGHT * 4];

    //for tracking overlapping sprites and sprite/bg/win priorities
    //pixel of color 1-3 from background/win has been drawn on current scanline
    std::bitset<DISPLAY_WIDTH> bg_win_1_3_visited_;
    //sprite pixel has been drawn on current scanline
    std::bitset<DISPLAY_WIDTH> obj_visited_;

    uint8_t vram_[VRAM_SPACE];
    uint8_t oam_[OAM_SPACE];
    //stores which oam sprites to draw on scanline
    uint16_t oam_objects[10];

    uint8_t LCDC_{0x91};
    uint8_t STAT_{0x85};
    uint8_t SCY_{0x00};
    uint8_t SCX_{0x00};
    uint8_t LY_{0x00};
    //internal line counter used by the window
    uint8_t win_ly_internal{0x00};
    uint8_t LYC_{0x00};
    uint8_t BGP_{0xFC};
    uint8_t OBP0_{0x00};
    uint8_t OBP1_{0x00};
    uint8_t WY_{0x00};
    uint8_t WX_{0x00};
};