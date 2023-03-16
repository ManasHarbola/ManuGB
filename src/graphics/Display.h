#pragma once
#include <cstdint>

/*
    Interface Class for Interacting w/ GUI library
    Manages rendering emulator video framebuffer and keyboard events
*/

class Display {

public:
    virtual ~Display(){}
    //updates output of request methods below and keyboard state
    virtual void poll_events() = 0;
    virtual void draw(const void* frameBuffer, uint32_t width,
                      uint32_t height, int pixel_size) const = 0;


    //All request methods are updated after calling poll_events()
    //GUI requests emulation to close
    virtual bool exit_requested() = 0;
    //GUI requests emulation to pause
    virtual bool pause_requested() = 0;
    //GUI requests emulator to save current state of game sram
    virtual bool save_requested() = 0;
};