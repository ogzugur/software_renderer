#pragma once
#include <iostream>
#include "utility_structs.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "texture.h"

class Shader
{
private:
    ogz_util::VertexData triangleVertices[3];

    glm::vec2 calculatePixelTexCoord(const glm::vec3 bc_screen);
    glm::vec3 calculateFragmentTNormal(const glm::vec3 bc_screen);

public:
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    Texture *textureDiffuse;
    Texture *textureNormal;

    void vertexShader(ogz_util::VertexData *triangleVertices);
    glm::vec3 fragmentShader(glm::vec3 fragmentNormal);
    Shader();
    ~Shader();
};
