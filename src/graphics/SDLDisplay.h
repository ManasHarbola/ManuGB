#pragma once

#include <string>
#include <graphics/Display.h>
#include <joypad/Joypad.h>
#include <SDL2/SDL.h>
#include <iostream>

class SDLDisplay : public Display, public Joypad {
public:
    SDLDisplay(std::string title, int width, int height, int& errcode);
    virtual ~SDLDisplay();
    virtual void poll_events() override;
    virtual void draw(const void* frameBuffer, uint32_t width,
                      uint32_t height, int pixel_size) const override;
    virtual bool exit_requested() override {return exit_requested_;}
    virtual bool pause_requested() override {return pause_requested_;}
    virtual bool save_requested() override {return save_requested_;}

    virtual void poll_joypad() override;
private:
    SDL_Window* window_{nullptr};
    SDL_Renderer* renderer_{nullptr};
    SDL_Texture* texture_{nullptr};
    SDL_Rect dst_rect;
    SDL_Event event_;

    const Uint8* keyboard_state_;
    bool exit_requested_{false};
    bool pause_requested_{false};
    bool save_requested_{false};
};