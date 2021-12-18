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
    };
}
