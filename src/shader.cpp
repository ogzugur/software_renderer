#include "shader.h"

Shader::Shader()
{
    std::cout << "Shader constructor\n";
}

Shader::~Shader()
{
    std::cout << "Shader destructor\n";
}

void Shader::vertexShader(ogz_util::VertexData &vertexData)
{
    glm::vec4 vertex4D = projectionMatrix * viewMatrix * modelMatrix * glm::vec4(vertexData.vertex_pos, 1.0f);
    vertexData.vertex_pos = glm::vec3(vertex4D) / vertex4D.w;
}

ogz_util::ColorRGB Shader::fragmentShader()
{
    ogz_util::ColorRGB fragColor;
    return fragColor;
}