#include <ppu/PPU.h>
#include <algorithm>

PPU::PPU(InterruptManager& int_manager):
int_manager_(int_manager) {
}

void PPU::tick() {
    //PPU only ticks when LCDC.7 == 1
    if (!ppu_enabled())
        return;

    t_cycle_lock_++;
    total_t_cycles_++;

    switch(get_ppu_state()) {
        case PPUState::OAM_FETCH:
            if (t_cycle_lock_ == FETCH_PERIOD) {
                t_cycle_lock_ = 0;
                set_ppu_state(PPUState::PIXEL_TRANSFER);
            }
            break;
        case PPUState::PIXEL_TRANSFER:
            //draw scanline
            if (t_cycle_lock_ == TRANSFER_PERIOD) {
                t_cycle_lock_ = 0;
                set_ppu_state(PPUState::HBLANK);
                check_for_irq();
                //updates the framebuffer, only if frame isn't skipped
                if (!skip_frame_)
                    draw_scanline();
            }
            break;
        case PPUState::HBLANK:
            if (t_cycle_lock_ == HBLANK_PERIOD) {
                //increment LY register
                t_cycle_lock_ = 0;
                inc_ly();
                update_coincidence_flag();

                if (LY_ == 144) {
                    set_ppu_state(PPUState::VBLANK);
                    check_for_irq(true);
                    int_manager_.request_interrupt(InterruptType::VBLANK);
                } else {
                    set_ppu_state(PPUState::OAM_FETCH);
                    check_for_irq();
                }
            }
            break;
        //VBLANK counts as scanlines, but nothing is drawn
        case PPUState::VBLANK:
            if (t_cycle_lock_ == VBLANK_LINE_PERIOD) {
                t_cycle_lock_ = 0;
                inc_ly();
                update_coincidence_flag();
                check_for_irq();

                if (LY_ == 0) {
                    win_ly_internal = 0;
                    set_ppu_state(PPUState::OAM_FETCH);
                    skip_frame_ = false;
                    //update_coincidence_flag();
                    check_for_irq();
                }
            }
            break;
    }
}

bool PPU::manages(uint16_t addr) {
    return (addr >= 0x8000 && addr < 0xA000) ||
           (addr >= 0xFE00 && addr < 0xFEA0) ||
           (addr >= 0xFF40 && addr < 0xFF46) || 
           (addr >= 0xFF47 && addr < 0xFF4C); 
}

uint8_t PPU::read(uint16_t addr) {
    //VRAM read - allowed when PPU not in PIXEL_TRANSFER mode
    if (addr >= 0x8000 && addr < 0xA000 && vram_accessible())
        return vram_[addr - 0x8000];
    //OAM read - allowed when PPU not in OAM_FETCH or PIXEL_TRANSFER mode
    if (addr >= 0xFE00 && addr < 0xFEA0 && oam_accessible())
        return oam_[addr - 0xFE00];
        
    //PPU Registers        
    switch(addr) {
        case 0xFF40:
            return LCDC_;
        case 0xFF41:
            return STAT_;
        case 0xFF42:
            return SCY_;
        case 0xFF43:
            return SCX_;
        case 0xFF44:
            return LY_;
        case 0xFF45:
            return LYC_;
        case 0xFF47:
            return BGP_;
        case 0xFF48:
            return OBP0_;
        case 0xFF49:
            return OBP1_;
        case 0xFF4A:
            return WY_;
        case 0xFF4B:
            return WX_;
    }
    //bad address
    //or CPU cannot access address right now
    return 0xFF;
}

void PPU::write(uint16_t addr, uint8_t val) {
    //VRAM write
    if (addr >= 0x8000 && addr < 0xA000 && vram_accessible()) {
        vram_[addr - 0x8000] = val;
        return;
    }
    //OAM write
    if (addr >= 0xFE00 && addr < 0xFEA0 && oam_accessible()) {
        oam_[addr - 0xFE00] = val;
        return;
    }
    //PPU Registers
    switch(addr) {
        case 0xFF40:
            {
                bool prev_enabled = ppu_enabled();
                LCDC_ = val;
                bool now_enabled = ppu_enabled();
                //LCDC_.7 decides whether PPU is enabled or not
                if (prev_enabled && !now_enabled)
                    disable_ppu();
                else if (!prev_enabled && now_enabled)
                    enable_ppu();
            }
            break;
        case 0xFF41:
            //LYC_STAT (Coincidence bit) and LCD_MODE<1:0> are read only
            //AND bit 7 is always reads 1
            STAT_ = 0x80 | (val & 0xF8) | (STAT_ & 0x7);
            check_for_irq();
            break;
        case 0xFF42:
            SCY_ = val;
            break;
        case 0xFF43:
            SCX_ = val;
            break;
        /*
        //LY is read-only for CPU
        case 0xFF44:
            LY_ = val;
            break;
        */
        case 0xFF45:
            LYC_ = val;
            if (ppu_enabled()) {
                update_coincidence_flag();
                check_for_irq();
            }
            break;
        case 0xFF47:
            BGP_ = val;
            break;
        case 0xFF48:
            OBP0_ = val;
            break;
        case 0xFF49:
            OBP1_ = val;
            break;
        case 0xFF4A:
            WY_ = val;
            break;
        case 0xFF4B:
            WX_ = val;
            break;
    }
}

void PPU::draw_scanline() {
    //TODO: May need to clear out screen to white
    //when LCDC & BG_WIN_ENABLE == 0

    //clear scanline bitsets
    bg_win_1_3_visited_.reset();
    obj_visited_.reset();

    draw_bg_scanline();
    draw_win_scanline();
    draw_obj_scanline();
}

/*
For drawing the background, we are doing the following for every pixel:

1. Given SCX_, SCY_, LY_, and pixel number in the row,
   find where the pixel is located (x,y) on the 256x256 Background tile grid
2. Given x,y, find the tile number in BG Grid for that pixel location
3. Use the tile number and the tile map to the find the address of the tile to draw from
4. Using y, and tile address, find the row of the tile to use
5. Using x, find the col of the tile to use
6. Draw pixel onto framebuffer using color map and BGP 
*/
void PPU::draw_bg_scanline() {
    if (LCDC_ & BG_WIN_ENABLE) {
        //pixel y pos wrt. BG grid
        uint8_t pix_y = SCY_ + LY_;
        //0=9800-9BFF, 1=9C00-9FFF
        const uint16_t map_addr = (LCDC_ & BG_TILE_MAP) ? 0x9C00 : 0x9800;
        const uint16_t data_addr = (LCDC_ & BG_WIN_TILE_DATA) ? 0x8000 : 0x9000;
        const bool use_signed = !((bool)(LCDC_ & BG_WIN_TILE_DATA));

        for (uint8_t px_cnt = 0; px_cnt < DISPLAY_WIDTH; px_cnt++) {
            //find x pos of pixel on 256x256 the background grid
            uint8_t pix_x = SCX_ + px_cnt;

            uint16_t tile_i = ((static_cast<uint16_t>(pix_y) >> 3) << 5) + (static_cast<uint16_t>(pix_x) >> 3);
            uint8_t tile_num = read(map_addr + tile_i);
            uint16_t tile_addr;
            //find memory address of tile to draw
            if (tile_num > 127 && use_signed) {
                tile_addr = data_addr - 0x800 + ((uint16_t(tile_num - 128)) << 4);
            } else {
                tile_addr = data_addr + ((uint16_t(tile_num)) << 4);
            }
            tile_addr += ((pix_y & 7) << 1);
            uint8_t tile_col = (pix_x & 7);
            uint8_t shift = (7 - tile_col);
            uint8_t lo_byte = read(tile_addr);
            uint8_t hi_byte = read(tile_addr + 1);
            uint8_t color_idx = (((hi_byte >> shift) & 1) << 1) | ((lo_byte >> shift) & 1);

            if (color_idx != 0)
                bg_win_1_3_visited_[px_cnt] = true;

            RGBA32 pix_color = COLOR_MAP[(BGP_ >> (color_idx << 1)) & 3];
            size_t idx = ((LY_ * DISPLAY_WIDTH) + px_cnt) * 4;
            frame_buffer_[idx]   = pix_color.r;
            frame_buffer_[idx+1] = pix_color.g;
            frame_buffer_[idx+2] = pix_color.b;
            frame_buffer_[idx+3] = pix_color.a;

            //move to next tile if we've finished drawing row from current tile
            if (tile_col == 7)
                tile_i++;
        }
    } else {
        //draw white
        for (uint8_t px_cnt = 0; px_cnt < DISPLAY_WIDTH; px_cnt++) {
            RGBA32 pix_color = COLOR_MAP[BGP_ & 3];
            size_t idx = ((LY_ * DISPLAY_WIDTH) + px_cnt) * 4;
            frame_buffer_[idx]   = pix_color.r;
            frame_buffer_[idx+1] = pix_color.g;
            frame_buffer_[idx+2] = pix_color.b;
            frame_buffer_[idx+3] = pix_color.a;
        }
    }
}

void PPU::draw_win_scanline() {
    if ((LCDC_ & BG_WIN_ENABLE) && (LCDC_ & WIN_ENABLE)) {
        if (LY_ < WY_ || WX_ > DISPLAY_WIDTH + 6)
            return;
        
        //calculate starting x pos of window wrt. screen coordinates
        int win_x = WX_ - 7;
        //internal window counter
        uint8_t win_cnt = win_ly_internal++;
        //first tile to start drawing window from
        uint16_t tile_i = (static_cast<uint16_t>(win_cnt) >> 3) << 5;

        const uint16_t map_addr = (LCDC_ & WIN_TILE_MAP) ? 0x9C00 : 0x9800; 
        const uint16_t data_addr = (LCDC_ & BG_WIN_TILE_DATA) ? 0x8000 : 0x9000;
        const bool use_signed = !((bool)(LCDC_ & BG_WIN_TILE_DATA));

        for (uint8_t frame_x = 0; frame_x < DISPLAY_WIDTH; frame_x++) {
            //check if window is drawn on pixel at frame_x
            if (frame_x < win_x || frame_x > win_x + DISPLAY_WIDTH)
                continue;

            //use tile_i to get tile # to draw from tile map
            uint8_t tile_num = read(map_addr + tile_i);
            uint16_t tile_addr;
            //find memory address of tile to draw
            if (tile_num > 127 && use_signed) {
                tile_addr = data_addr - 0x800 + ((uint16_t(tile_num - 128)) << 4);
            } else {
                tile_addr = data_addr + ((uint16_t(tile_num)) << 4);
            }
            
            //get correct pixel row from tile
            //Note: 2 bytes/row of pixels in tile
            tile_addr += ((win_cnt & 7) << 1);
            //get column of pixel in tile we need to draw
            //0-indexed from left to right
            uint8_t tile_col = (frame_x - win_x) & 7;
            uint8_t shift = (7 - tile_col);
            uint8_t lo_byte = read(tile_addr);
            uint8_t hi_byte = read(tile_addr + 1);
            uint8_t color_idx = (((hi_byte >> shift) & 1) << 1) | ((lo_byte >> shift) & 1);

            if (color_idx != 0)
                bg_win_1_3_visited_[frame_x] = true;

            RGBA32 pix_color = COLOR_MAP[(BGP_ >> (color_idx << 1)) & 3];
            size_t idx = ((LY_ * DISPLAY_WIDTH) + frame_x) * 4;
            frame_buffer_[idx] = pix_color.r;
            frame_buffer_[idx + 1] = pix_color.g;
            frame_buffer_[idx + 2] = pix_color.b;
            frame_buffer_[idx + 3] = pix_color.a;
            //move to next tile if we've finished drawing row from current tile
            if (tile_col == 7)
                tile_i++;
        }
    }
}

void PPU::draw_obj_scanline() {
    if (LCDC_ & OBJ_ENABLE) {
        uint8_t num_sprites = 0;
        /*
        Check each oam entry and decide which need to be drawn
        onto scanline
        */
        for (uint16_t addr = 0xFE00; addr < 0xFEA0 && num_sprites < MAX_SPRITES_LINE; addr += 4) {
            int y_pos = read(addr);
            
            if (((int) LY_) >= (y_pos - 16)) {
                if (((LCDC_ & OBJ_SIZE_FLAG) && ((int) LY_) < y_pos) ||
                    (((int) LY_) < y_pos - 8)) {
                    oam_objects[num_sprites++] = addr;
                }
            }

        }
        //for debugging, delete later
        
        //std::cout << "LY_ " << (int) LY_ << "has " << (int) num_sprites << " sprites" << std::endl;

        if (num_sprites > 0) {
            /*
            order sprites by increasing priority
            so subsequent sprites that overlap will overrite lower sprites that
            have already been written
            */
            std::sort(oam_objects, oam_objects + num_sprites,
                      [this](uint16_t a1, uint16_t a2){
                                return (read(a1 + 1) < read(a2 + 1)) ||
                                       (read(a1 + 1) == read(a2 + 1) && a1 < a2);
                            }
            );
            
            for (int i = 0; i < num_sprites; i++) {
                int y_pos = static_cast<int>(read(oam_objects[i])) - 16;
                int x_pos = static_cast<int>(read(oam_objects[i] + 1)) - 8;
                uint16_t tile_i = read(oam_objects[i] + 2);
                uint8_t attr_flags = read(oam_objects[i] + 3);

                uint16_t line_num = static_cast<uint16_t>(((int) LY_) - y_pos);

                if ((LCDC_ & OBJ_SIZE_FLAG)) {
                    tile_i &= 0xFE;
                    if (attr_flags & FLIP_SPRITE_Y) {
                        line_num = 15 - line_num;
                    }
                    if (line_num > 7) {
                        tile_i++;
                        line_num -= 8;
                    }
                } else if (attr_flags & FLIP_SPRITE_Y) {
                    line_num = 7 - line_num;
                }
                
                uint16_t tile_addr = 0x8000 + (tile_i << 4) + (line_num << 1);

                uint8_t lo_byte = read(tile_addr);
                uint8_t hi_byte = read(tile_addr + 1);
                uint8_t palette = (attr_flags & SPRITE_PALETTE) ? OBP1_ : OBP0_;
                
                int8_t cnt = (attr_flags & FLIP_SPRITE_X) ? 0 : 7;

                while (cnt >= 0 && cnt < 8 && x_pos < DISPLAY_WIDTH) {
                    if (x_pos >= 0) {
                        uint8_t color_idx = ((((hi_byte & (1 << cnt)) >> cnt) << 1) | ((lo_byte & (1 << cnt)) >> cnt)) & 3;
                        if (!obj_visited_[x_pos]) {
                            //draw sprite on line only if neither condition is satisfied
                            if (!(color_idx == 0 ||
                                    ((attr_flags & BG_WIN_OVER_OBJ) && bg_win_1_3_visited_[x_pos]) ||
                                 !(LCDC_ & BG_WIN_ENABLE))
                                ) {
                                RGBA32 pix_color = COLOR_MAP[(palette >> (color_idx << 1)) & 3];
                                size_t idx = ((LY_ * DISPLAY_WIDTH) + x_pos) * 4;
                                frame_buffer_[idx]   = pix_color.r;
                                frame_buffer_[idx+1] = pix_color.g;
                                frame_buffer_[idx+2] = pix_color.b;
                                frame_buffer_[idx+3] = pix_color.a;
                            }
                            //mark pixel as visited
                            obj_visited_[x_pos] = true;
                        }
                    }
                    x_pos++;
                    cnt = (attr_flags & FLIP_SPRITE_X) ? (cnt + 1) : (cnt - 1);
                }
            }
        }
    }
}