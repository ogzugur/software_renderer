#include "frame.h"

void Frame::writeFile(const char *file_name, bool is_flipped)
{
    stbi_flip_vertically_on_write(is_flipped);
    stbi_write_png(file_name, width, height, 3, color_buffer, width * 3);
}

bool Frame::setPixel(unsigned int x, unsigned int y, glm::vec3 color)
{
    *(this->color_buffer + 3 * (y * width + x)) = color.x;
    *(this->color_buffer + 3 * (y * width + x) + 1) = color.y;
    *(this->color_buffer + 3 * (y * width + x) + 2) = color.z;

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

void Frame::clearDepthColor()
{
    free(color_buffer);
    free(depth_buffer);
    this->color_buffer = (unsigned char *)calloc(width * height * 3, sizeof(unsigned char));
    this->depth_buffer = (float *)calloc(width * height, sizeof(float));
    for (int i = width * height; i--; depth_buffer[i] = 1.0f)
        ;
}

Frame::Frame(unsigned int width, unsigned int height)
{
    this->color_buffer = (unsigned char *)calloc(width * height * 3, sizeof(unsigned char));
    this->depth_buffer = (float *)calloc(width * height, sizeof(float));
    for (int i = width * height; i--; depth_buffer[i] = 1.0f)
        ;
    this->width = width;
    this->height = height;
}

Frame::~Frame()
{
    free(color_buffer);
    free(depth_buffer);
}
