#pragma once
#include <glm/glm.hpp>

namespace ogz_util
{
    struct ColorRGB
    {
        unsigned char r, g, b;
        ColorRGB() : b(0), g(0), r(0) {}
        ColorRGB(unsigned char R, unsigned char G, unsigned char B) : b(B), g(G), r(R) {}
    };

    struct VertexData
    {
        glm::vec3 vertex_pos;
        glm::vec2 vertex_tex_coord;
        glm::vec3 vertex_normal;
        glm::vec3 vertex_tangent;
        glm::vec3 vertex_bitangent;
    };

    typedef enum
    {
        TEXTURE_TYPE_AMBIENT, // default
        TEXTURE_TYPE_DIFFUSE,
        TEXTURE_TYPE_SPECULAR,
        TEXTURE_TYPE_BUMP,
        TEXTURE_TYPE_DISPLACEMENT,
        TEXTURE_TYPE_ALPHA,
        TEXTURE_TYPE_ROUGHNESS,
        TEXTURE_TYPE_METALLIC,
        TEXTURE_TYPE_NORMAL,
        TEXTURE_TYPE_EMISSIVE
    } textureType;
}
