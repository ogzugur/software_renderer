#include "renderer.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer(Camera &camera, Frame &frame)
{
    this->camera = &camera;
    this->frame = &frame;
}

Renderer::~Renderer()
{
}

void Renderer::drawModel(Model &model)
{
    rotateValue += 1;

    // Loop over shapes
    for (size_t s = 0; s < model.meshes.size(); s++)
    {
        for (size_t i = 0; i < model.meshes[s].vertices.size(); i = i + 3)
        {
            //draw triangle
            rendererShader.textureDiffuse = &model.meshes[s].textures[0];
            drawTriangle(model.meshes[s].vertices[i + 0],
                         model.meshes[s].vertices[i + 1],
                         model.meshes[s].vertices[i + 2], model.meshes[s].textures[0]);
        }
    }
}

//bresenham line drawing algorithm
void Renderer::plotLine(glm::vec2 p0, glm::vec2 p1, glm::vec3 color)
{
    int dx = (int)abs(p1.x - p0.x), sx = p0.x < p1.x ? 1 : -1;
    int dy = -(int)abs(p1.y - p0.y), sy = p0.y < p1.y ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;)
    { /* loop */
        frame->setPixel((unsigned int)p0.x, (unsigned int)p0.y, color);
        if (p0.x == p1.x && p0.y == p1.y)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            p0.x += sx;
        } /* e_xy+e_x > 0 */
        if (e2 <= dx)
        {
            err += dx;
            p0.y += sy;
        } /* e_xy+e_y < 0 */
    }
}

// Compute barycentric coordinates (u, v, w) for
// point p with respect to triangle (a, b, c)
glm::vec3 Renderer::barycentric(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
    float u, v, w;
    glm::vec2 v0 = b - a, v1 = c - a, v2 = p - a;
    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
    return glm::vec3(u, v, w);
}

void Renderer::viewportTransform(glm::vec4 &p, int viewport_width, int viewport_height)
{
    p.x = (int)((p.x + 1) / 2 * (viewport_width - 1));
    p.y = (int)((p.y + 1) / 2 * (viewport_height - 1));
}

float Renderer::calculatePixelDepth(const glm::vec3 bc_screen,
                                    const glm::vec4 p0,
                                    const glm::vec4 p1,
                                    const glm::vec4 p2)
{
    float pixel_depth = 0;
    pixel_depth = p0.z * bc_screen.x +
                  p1.z * bc_screen.y +
                  p2.z * bc_screen.z;

     return pixel_depth;
}

void Renderer::drawTriangle(ogz_util::VertexData p0, ogz_util::VertexData p1, ogz_util::VertexData p2, Texture &texture)
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    ogz_util::VertexData triangleVertices[3] = {p0,p1,p2};

    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0));
    //modelMatrix = glm::rotate(modelMatrix,glm::radians(rotateValue),glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 viewMatrix = camera->getCameraViewMatrix();
    glm::mat4 projectionMatrix = camera->getCameraProjectionMatrix();

    rendererShader.modelMatrix = modelMatrix;
    rendererShader.viewMatrix = viewMatrix;
    rendererShader.projectionMatrix = projectionMatrix;

    rendererShader.vertexShader(triangleVertices);

    viewportTransform(triangleVertices[0].vertex_pos, this->frame->getWidth(), this->frame->getHeight());
    viewportTransform(triangleVertices[1].vertex_pos, this->frame->getWidth(), this->frame->getHeight());
    viewportTransform(triangleVertices[2].vertex_pos, this->frame->getWidth(), this->frame->getHeight());

    glm::vec2 bboxmin(this->frame->getWidth() - 1, this->frame->getHeight() - 1);
    glm::vec2 bboxmax(0, 0);
    glm::vec2 clamp(this->frame->getWidth() - 1, this->frame->getHeight() - 1);

    bboxmin.x = std::max(0.0f, std::min(triangleVertices[0].vertex_pos.x, std::min(triangleVertices[1].vertex_pos.x, triangleVertices[2].vertex_pos.x)));
    bboxmin.y = std::max(0.0f, std::min(triangleVertices[0].vertex_pos.y, std::min(triangleVertices[1].vertex_pos.y, triangleVertices[2].vertex_pos.y)));

    bboxmax.x = std::min(clamp.x, std::max(triangleVertices[0].vertex_pos.x, std::max(triangleVertices[1].vertex_pos.x, triangleVertices[2].vertex_pos.x)));
    bboxmax.y = std::min(clamp.y, std::max(triangleVertices[0].vertex_pos.y, std::max(triangleVertices[1].vertex_pos.y, triangleVertices[2].vertex_pos.y)));

    glm::vec3 P;
    glm::vec3 fragmentColor;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            float pixel_depth_value = this->frame->getDepthBufferValue(P.x, P.y);
            glm::vec3 bc_screen = barycentric(P, triangleVertices[0].vertex_pos, triangleVertices[1].vertex_pos, triangleVertices[2].vertex_pos);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                continue;
            P.z = calculatePixelDepth(bc_screen, triangleVertices[0].vertex_pos, triangleVertices[1].vertex_pos, triangleVertices[2].vertex_pos);

            if (pixel_depth_value > P.z)
            {
                this->frame->setDepthBufferValue(P.x, P.y, P.z);
                fragmentColor = rendererShader.fragmentShader(bc_screen);
                this->frame->setPixel(P.x, P.y, fragmentColor);
            }
        }
    }
}
