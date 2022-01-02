#include "shader.h"

Shader::Shader()
{
    std::cout << "Shader constructor\n";
}

Shader::~Shader()
{
    std::cout << "Shader destructor\n";
}


glm::vec2 Shader::calculatePixelTexCoord(const glm::vec3 bc_screen)
{
    glm::vec2 texCoord;
    texCoord.x = this->triangleVertices[0].vertex_tex_coord.x * bc_screen.x +
                 this->triangleVertices[1].vertex_tex_coord.x * bc_screen.y +
                 this->triangleVertices[2].vertex_tex_coord.x * bc_screen.z;

    texCoord.y = this->triangleVertices[0].vertex_tex_coord.y * bc_screen.x +
                 this->triangleVertices[1].vertex_tex_coord.y * bc_screen.y +
                 this->triangleVertices[2].vertex_tex_coord.y * bc_screen.z;

    return texCoord;
}

glm::vec3 Shader::calculateFragmentTNormal(const glm::vec3 bc_screen)
{
    glm::vec3 fragmentNormal;
    fragmentNormal = bc_screen.x * this->triangleVertices[0].vertex_normal + bc_screen.y * this->triangleVertices[1].vertex_normal + bc_screen.z * this->triangleVertices[2].vertex_normal;
    fragmentNormal = glm::normalize(fragmentNormal);

    return fragmentNormal;
}

void Shader::vertexShader(ogz_util::VertexData *triangleVertices)
{
    for (size_t i = 0; i < 3; i++)
    {
        this->triangleVertices[i] = triangleVertices[i];
        glm::vec4 vertex4D = projectionMatrix * viewMatrix * modelMatrix * glm::vec4(triangleVertices[i].vertex_pos, 1.0f);
        triangleVertices[i].vertex_pos = glm::vec3(vertex4D) / vertex4D.w;
    }
}

glm::vec3 Shader::fragmentShader(glm::vec3 barycentricCoordinate)
{
    glm::vec3 fragColor = glm::vec3(255, 255, 255);
    glm::vec3 fragmentNormal = calculateFragmentTNormal(barycentricCoordinate);
    glm::vec2 texCoord = calculatePixelTexCoord(barycentricCoordinate);
    glm::vec3 textureColor = textureDiffuse->getColorValueByUV(texCoord.x, texCoord.y);

    fragmentNormal = glm::vec3(modelMatrix * glm::vec4(fragmentNormal, 0.0f));
    float intensity = glm::dot(glm::vec3(fragmentNormal), glm::normalize(glm::vec3(1.0, 0.0, 0.0)));
    if (intensity < 0)
        intensity = 0;

    fragColor = intensity * textureColor;

    return fragColor;
}