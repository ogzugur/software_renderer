#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include <iostream>

struct ColorRGB
{
    unsigned char r, g, b;
    ColorRGB(unsigned char R, unsigned char G, unsigned char B) : b(B), g(G), r(R) {}
};

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

    bool set_pixel(unsigned int x, unsigned int y, ColorRGB color);
    void set_dept_buffer_value(unsigned x, unsigned y, float depth_value);
    float get_depth_buffer_value(unsigned x, unsigned y);
    void write_file(const char *file_name, bool is_flipped = false);
    int get_width();
    int get_height();
};

void Frame::write_file(const char *file_name, bool is_flipped)
{
    stbi_flip_vertically_on_write(is_flipped);
    stbi_write_png(file_name, width, height, 3, color_buffer, width * 3);
    free(color_buffer);
}

bool Frame::set_pixel(unsigned int x, unsigned int y, ColorRGB color)
{
    *(this->color_buffer + 3 * (y * width + x)) = color.r;
    *(this->color_buffer + 3 * (y * width + x) + 1) = color.g;
    *(this->color_buffer + 3 * (y * width + x) + 2) = color.b;

    return true;
}

void Frame::set_dept_buffer_value(unsigned x, unsigned y, float depth_value)
{
    *(this->depth_buffer + (y * width + x)) = depth_value;
}

float Frame::get_depth_buffer_value(unsigned x, unsigned y)
{
    return *(this->depth_buffer + (y * width + x));
}

int Frame::get_width()
{
    return width;
}

int Frame::get_height()
{
    return height;
}

Frame::Frame(unsigned int width, unsigned int height)
{
    this->color_buffer = (unsigned char *)calloc(width * height * 3, sizeof(unsigned char));
    this->depth_buffer = (float *)calloc(width * height, sizeof(float));
    for (int i=width*height; i--; depth_buffer[i] = -std::numeric_limits<float>::max());
    this->width = width;
    this->height = height;
}

Frame::~Frame()
{
}
