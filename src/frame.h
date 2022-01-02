#pragma once
#include <iostream>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include "utility_structs.h"

class Frame
{
private:
    /* data */
    float *depth_buffer;
    unsigned int width, height;

public:
    unsigned char *color_buffer;
    Frame();
    Frame(unsigned int width, unsigned int height);
    ~Frame();

    int getWidth();
    int getHeight();
    void clearDepthColor();
    float getDepthBufferValue(unsigned x, unsigned y);
    void writeFile(const char *file_name, bool is_flipped = false);
    void setDepthBufferValue(unsigned x, unsigned y, float depth_value);
    bool setPixel(unsigned int x, unsigned int y, glm::vec3 color);
};