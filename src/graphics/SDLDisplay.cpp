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

    keyboard_state_ = SDL_GetKeyboardState(NULL);
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

void SDLDisplay::poll_events() {
    //poll all events here
    while (SDL_PollEvent(&event_)) {
        switch (event_.type) {
            case SDL_QUIT:
                exit_requested_ = true;
                break;
            case SDL_KEYDOWN:
                if (event_.key.keysym.scancode == SDL_SCANCODE_P)
                    pause_requested_ = !pause_requested_;
                break;
        }
    }
}

void SDLDisplay::poll_joypad() {
    dpad_ |= 0x0F;
    action_ |= 0x0F;

    //const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    if (keyboard_state_[SDL_SCANCODE_W])
        dpad_ &= ~(1 << 2);
    if (keyboard_state_[SDL_SCANCODE_A])
        dpad_ &= ~(1 << 1);
    if (keyboard_state_[SDL_SCANCODE_S])
        dpad_ &= ~(1 << 3);
    if (keyboard_state_[SDL_SCANCODE_D])
        dpad_ &= ~(1 << 0);

    if (keyboard_state_[SDL_SCANCODE_J])
        action_ &= ~(1 << 1);
    if (keyboard_state_[SDL_SCANCODE_K])
        action_ &= ~(1 << 0);
    if (keyboard_state_[SDL_SCANCODE_N])
        action_ &= ~(1 << 3);
    if (keyboard_state_[SDL_SCANCODE_M])
        action_ &= ~(1 << 2);
}