#pragma once
#include <stb/stb_image.h>
#include "utility_structs.h"

class Texture
{
private:
    unsigned char *texture_data;
    int width, height, nrChannels;

public:
    Texture(/* args */);
    Texture(const char *file_name);
    ~Texture();

public:
    ogz_util::ColorRGB getColorValueByUV(float u, float v);
};
