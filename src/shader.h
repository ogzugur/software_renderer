#pragma once
#include <iostream>
#include "utility_structs.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "texture.h"

class Shader
{
private:
public:
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    Texture *texture;

    void vertexShader(ogz_util::VertexData &vertexData);
    ogz_util::ColorRGB fragmentShader();
    Shader();
    ~Shader();
};
