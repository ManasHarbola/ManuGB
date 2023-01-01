#pragma once

#include <cstdint>

class Display {

public:
    virtual ~Display(){}
    virtual void draw(const void* frameBuffer, uint32_t width,
                      uint32_t height, int pixel_size) const = 0;
};