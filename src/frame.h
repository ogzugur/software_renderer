#pragma once
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include <iostream>
#include "utility_structs.h"
class Frame
{
private:
    /* data */
    unsigned char *color_buffer;
    float *depth_buffer;
    unsigned int width, height;

public:
    Frame(unsigned int width, unsigned int height);
    ~Frame();

    bool setPixel(unsigned int x, unsigned int y, ogz_util::ColorRGB color);
    void setDepthBufferValue(unsigned x, unsigned y, float depth_value);
    float getDepthBufferValue(unsigned x, unsigned y);
    void writeFile(const char *file_name, bool is_flipped = false);
    int getWidth();
    int getHeight();
};

void Frame::writeFile(const char *file_name, bool is_flipped)
{
    stbi_flip_vertically_on_write(is_flipped);
    stbi_write_png(file_name, width, height, 3, color_buffer, width * 3);
    free(color_buffer);
}

bool Frame::setPixel(unsigned int x, unsigned int y, ogz_util::ColorRGB color)
{
    *(this->color_buffer + 3 * (y * width + x)) = color.r;
    *(this->color_buffer + 3 * (y * width + x) + 1) = color.g;
    *(this->color_buffer + 3 * (y * width + x) + 2) = color.b;

    return true;
}

void Frame::setDepthBufferValue(unsigned x, unsigned y, float depth_value)
{
    *(this->depth_buffer + (y * width + x)) = depth_value;
}

float Frame::getDepthBufferValue(unsigned x, unsigned y)
{
    return *(this->depth_buffer + (y * width + x));
}

int Frame::getWidth()
{
    return width;
}

int Frame::getHeight()
{
    return height;
}

Frame::Frame(unsigned int width, unsigned int height)
{
    this->color_buffer = (unsigned char *)calloc(width * height * 3, sizeof(unsigned char));
    this->depth_buffer = (float *)calloc(width * height, sizeof(float));
    for (int i = width * height; i--; depth_buffer[i] = -std::numeric_limits<float>::max())
        ;
    this->width = width;
    this->height = height;
}

Frame::~Frame()
{
}
