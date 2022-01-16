#pragma once
#include "utility_structs.h"
#include "texture.h"
#include "frame.h"
#include "camera.h"
#include "model.h"
#include "shader.h"

class Renderer
{
private:
    glm::vec3 barycentric(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c);
    void viewportTransform(glm::vec4 &p, int viewport_width, int viewport_height);
    void plotLine(glm::vec2 p0, glm::vec2 p1, glm::vec3 color);

    float calculatePixelDepth(const glm::vec3 bc_screen,
                              const glm::vec4 p0,
                              const glm::vec4 p1,
                              const glm::vec4 p2);

    void drawTriangle(ogz_util::VertexData p0,
                      ogz_util::VertexData p1,
                      ogz_util::VertexData p2,
                      Texture &texture);

public:
    Camera *camera;
    Frame *frame;
    Shader rendererShader;

    Renderer(){};
    Renderer(Camera &camera, Frame &frame);
    ~Renderer();
    void drawModel(Model &model);
};
