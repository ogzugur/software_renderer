#include "texture.h"

Texture::Texture(/* args */)
{
}

Texture::Texture(const char *file_name)
{
    stbi_set_flip_vertically_on_load(true);
    texture_data = stbi_load(file_name, &width, &height, &nrChannels, 0);
}

//Returns [0-255] range color value
ogz_util::ColorRGB Texture::getColorValueByUV(float u, float v)
{
    ogz_util::ColorRGB colorValue;

    unsigned int i = (int)(u * width + 0.5f);
    unsigned int j = (int)(v * height + 0.5f);

    unsigned char *pixelOffset = texture_data + (i + width * j) * nrChannels;
    colorValue.r = pixelOffset[0];
    colorValue.g = pixelOffset[1];
    colorValue.b = pixelOffset[2];

    return colorValue;
}

Texture::~Texture()
{
}
