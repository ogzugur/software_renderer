#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <iostream>
class Image
{
private:
    /* data */
    unsigned char *data;
    unsigned int width, height;

public:
    Image(unsigned int width, unsigned int height);
    ~Image();

    void set_pixel(unsigned int x, unsigned int y, unsigned char R, unsigned char G, unsigned char B);
    void write_file(const char *file_name);
};

void Image::write_file(const char *file_name)
{
    stbi_write_jpg(file_name, width, height, 3, data, 100);
}

void Image::set_pixel(unsigned int x, unsigned int y, unsigned char R, unsigned char G, unsigned char B)
{
    if (x < width && y < height)
    {
        *(this->data + 3 * (y * width + x)) = R;
        *(this->data + 3 * (y * width + x) + 1) = G;
        *(this->data + 3 * (y * width + x) + 2) = B;
    }
    else
    {
        printf("Pixel position is out of range: Pos -> %d %d", x, y);
    }
}

Image::Image(unsigned int width, unsigned int height)
{
    this->data = (unsigned char *)malloc(width * height * 3);
    this->width = width;
    this->height = height;
}

Image::~Image()
{
}
