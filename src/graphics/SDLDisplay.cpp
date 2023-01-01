#include <graphics/SDLDisplay.h>
#include <Constants.h>

SDLDisplay::SDLDisplay(std::string title, int width, int height, int& errcode) : 
dst_rect({0, 0, width, height}) {
    errcode = 0;
    window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, width, height,
                               SDL_WINDOW_SHOWN);
    if (!window_) {
        std::cout << SDL_GetError() << std::endl;
        errcode = -2;
        return;
    }
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!renderer_) {
        std::cout << SDL_GetError() << std::endl;
        errcode = -3;
        return;
    }
    //TODO: Get rid of this hard coding
    texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA32,
                                 SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if (!texture_) {
        std::cout << SDL_GetError() << std::endl;
        errcode = -4;
        return;
    }
}

SDLDisplay::~SDLDisplay() {
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}

void SDLDisplay::draw(const void* frameBuffer, uint32_t width,
                      uint32_t height, int pixel_size) const {

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0);
    SDL_RenderClear(renderer_);
    SDL_UpdateTexture(texture_, NULL, frameBuffer, width * pixel_size);
    SDL_RenderCopy(renderer_, texture_, NULL, &dst_rect);
    SDL_RenderPresent(renderer_);
}