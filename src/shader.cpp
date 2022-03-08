#include "shader.h"

Shader::Shader()
{
    std::cout << "Shader constructor\n";
}

Shader::~Shader()
{
    std::cout << "Shader destructor\n";
}

glm::vec2 Shader::calculatePixelTexCoord(const glm::vec3 bc_clip)
{
    glm::vec2 texCoord;
    texCoord.x = this->triangleVertices[0].vertex_tex_coord.x * bc_clip.x +
                 this->triangleVertices[1].vertex_tex_coord.x * bc_clip.y +
                 this->triangleVertices[2].vertex_tex_coord.x * bc_clip.z;

    texCoord.y = this->triangleVertices[0].vertex_tex_coord.y * bc_clip.x +
                 this->triangleVertices[1].vertex_tex_coord.y * bc_clip.y +
                 this->triangleVertices[2].vertex_tex_coord.y * bc_clip.z;

    return texCoord;
}

glm::vec3 Shader::calculateFragmentTNormal(const glm::vec3 bc_clip)
{
    glm::vec3 fragmentNormal;
    fragmentNormal = bc_clip.x * this->triangleVertices[0].vertex_normal + bc_clip.y * this->triangleVertices[1].vertex_normal + bc_clip.z * this->triangleVertices[2].vertex_normal;
    fragmentNormal = glm::normalize(fragmentNormal);

    return fragmentNormal;
}

void Shader::vertexShader(ogz_util::VertexData *triangleVertices)
{
    for (size_t i = 0; i < 3; i++)
    {
        this->triangleVertices[i] = triangleVertices[i];
        glm::vec4 vertex4D = projectionMatrix * viewMatrix * modelMatrix * triangleVertices[i].vertex_pos;
        triangleVertices[i].vertex_pos = vertex4D / vertex4D.w;
        this->vertexPosClipSpace[i] = vertex4D;
        triangleVertices[i].vertex_pos.w = vertex4D.w;
    }
}

glm::vec3 Shader::fragmentShader(glm::vec3 barycentricCoordinate)
{
    glm::vec3 bc_clip = glm::vec3(barycentricCoordinate.x / this->vertexPosClipSpace[0].w, barycentricCoordinate.y /this->vertexPosClipSpace[1].w, barycentricCoordinate.z / this->vertexPosClipSpace[2].w);
    bc_clip = bc_clip / (bc_clip.x + bc_clip.y + bc_clip.z);
    glm::vec3 fragColor = glm::vec3(255, 255, 255);
    glm::vec3 fragmentNormal = calculateFragmentTNormal(bc_clip);
    glm::vec2 texCoord = calculatePixelTexCoord(bc_clip);
    glm::vec3 textureColor = textureDiffuse->getColorValueByUV(texCoord.x, texCoord.y);

    fragmentNormal = glm::vec3(modelMatrix * glm::vec4(fragmentNormal, 0.0f));
    float intensity = glm::dot(glm::vec3(fragmentNormal), glm::normalize(glm::vec3(1.0, 1.0, 0.0)));
    if (intensity < 0)
        intensity = 0;

    fragColor = textureColor;

    return fragColor;
}