#pragma once
#include <cstdint>

/*
    Interface Class for interacting GUI library

    Manages rendering framebuffer and managing keyboard/mouse events
*/

class Display {

public:
    virtual ~Display(){}
    virtual void poll_events() = 0;
    virtual void draw(const void* frameBuffer, uint32_t width,
                      uint32_t height, int pixel_size) const = 0;
    //GUI requests emulation to close
    virtual bool exit_requested() = 0;
    //GUI requests emulation to pause
    virtual bool pause_requested() = 0;
};