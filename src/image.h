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

class Image
{
private:
    /* data */
    unsigned char *data;
    unsigned int width, height;

public:
    Image(unsigned int width, unsigned int height);
    ~Image();

    bool set_pixel(unsigned int x, unsigned int y, ColorRGB color);
    void write_file(const char *file_name);
};

void Image::write_file(const char *file_name)
{
    stbi_flip_vertically_on_write(true);
    stbi_write_jpg(file_name, width, height, 3, data, 100);
    free(data);
}

bool Image::set_pixel(unsigned int x, unsigned int y, ColorRGB color)
{
    *(this->data + 3 * (y * width + x)) = color.r;
    *(this->data + 3 * (y * width + x) + 1) = color.g;
    *(this->data + 3 * (y * width + x) + 2) = color.b;

    return true;
}

Image::Image(unsigned int width, unsigned int height)
{
    this->data = (unsigned char *)calloc(width * height * 3, sizeof(unsigned char));
    this->width = width;
    this->height = height;
}

Image::~Image()
{
}
