#pragma once
#include <stb/stb_image.h>
#include <string>
#include "utility_structs.h"

class Texture
{
private:
    int width, height, nrChannels;

public:
    unsigned char *texture_data;
    Texture(/* args */);
    Texture(const char *file_name);
    ~Texture();

public:
    glm::vec3 getColorValueByUV(float u, float v);
    std::string path;
    ogz_util::textureType type;
};
