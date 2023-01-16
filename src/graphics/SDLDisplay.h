#pragma once

#include <string>
#include <graphics/Display.h>
#include <SDL2/SDL.h>
#include <iostream>

class SDLDisplay : public Display {
public:
    SDLDisplay(std::string title, int width, int height, int& errcode);
    virtual ~SDLDisplay();
    virtual void draw(const void* frameBuffer, uint32_t width,
                      uint32_t height, int pixel_size) const override;
private:
    SDL_Window* window_{nullptr};
    SDL_Renderer* renderer_{nullptr};
    SDL_Texture* texture_{nullptr};
    SDL_Rect dst_rect;

};