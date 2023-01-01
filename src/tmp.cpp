#include <iostream>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <thread>
#include <string>
#include <SDL2/SDL.h>
#include <cstdio>

using namespace std::chrono;
using std::this_thread::sleep_for;
//void updateBuffer()

void fillBufferRand(char *buffer, size_t sz) {
    std::ifstream rand_file("/dev/random");
    if (rand_file.is_open())
        rand_file.read(buffer, sz);
}

void tweakBuffer(char *buffer, size_t sz) {
    int val = std::rand();
    size_t start = std::rand() % sz;
    size_t cnt;
    while ((cnt = (std::rand() % (sz - start))) == 0) {}
    std::fill(buffer + start, buffer + start + cnt - 1, val);
}

int main() {
    //std::printf("%0X\n", SDL_MapRGBA(&SDL_PIXELFORMAT_RGBA32, 0, 0, 0, 0xff));

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "init error: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Window* window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, 640, 480,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "window error: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "renderer error: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                             SDL_TEXTUREACCESS_STREAMING, 640, 480);
    if (!texture) {
        std::cout << "texture error: " << SDL_GetError() << std::endl;
        return -1;
    }


    SDL_RenderPresent(renderer);
    SDL_Event e;

    char buffer[640 * 480 * 4]{0};
    fillBufferRand(buffer, 640 * 480 * 4);

    bool running = true;
    while (running) {
        //Uint32 start = SDL_GetPerformanceCounter();
        high_resolution_clock::time_point start = high_resolution_clock::now(); 

        if (SDL_PollEvent(&e) != 0) {
            switch(e.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    std::cout << "key press detected" << std::endl;
                    break;
            }
        }

        //change framebuffer
        //tweakBuffer(buffer, 640 * 480 * 4);
        
        //clear out renderer
        SDL_SetRenderDrawColor(renderer, 0xc6, 0xb7, 0xbe, 0xff);

        SDL_RenderClear(renderer);

        //update texture
        //SDL_UpdateTexture(texture, NULL, buffer, 640 * 4);

        //copy texture to renderer
        //SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);

        high_resolution_clock::time_point end = high_resolution_clock::now();
        auto time_span = std::chrono::duration_cast<microseconds>(end - start);
        
        //60 frames / sec -> 16.66666 ms / frame
        //                -> 16666.66 microseconds / frame
        constexpr double fps_duration_microsec = (1e6 / 60.0);
        double wait_microsec = fps_duration_microsec - time_span.count();
        //remove 1.9ms
        wait_microsec -= 1190;
        //std::cout << "It took " << wait_msec << "ms to perform " << CYCLES_PER_FRAME << " t-cycles" << std::endl;
        if (wait_microsec > 0)
            sleep_for(microseconds((int) (wait_microsec)));

        high_resolution_clock::time_point end2 = high_resolution_clock::now();
        duration<double, std::milli> time_span_final = end2 - start;
        std::cout << "FPS: " << std::to_string(1000.0 / time_span_final.count()) << std::endl;
    }

    //cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}