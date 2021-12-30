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
    rotateValue += 2;
    // Loop over shapes
    for (size_t s = 0; s < model.shapes.size(); s++)
    {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < model.shapes[s].mesh.num_face_vertices.size(); f++)
        {
            size_t fv = size_t(model.shapes[s].mesh.num_face_vertices[f]);
            ogz_util::VertexData triangle_vertices[3];
            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++)
            {
                // access to vertex
                tinyobj::index_t idx = model.shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = model.attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = model.attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = model.attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                //viewport transformation
                triangle_vertices[v].vertex_pos = glm::vec3(vx, vy, vz);

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0)
                {
                    tinyobj::real_t nx = model.attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = model.attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = model.attrib.normals[3 * size_t(idx.normal_index) + 2];
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0)
                {
                    tinyobj::real_t tx = model.attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = model.attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    triangle_vertices[v].vertex_tex_coord = glm::vec2(tx, ty);
                }
            }
            //draw triangle
            drawTriangle(triangle_vertices[0],
                         triangle_vertices[1],
                         triangle_vertices[2], *model.texture);

            index_offset += fv;
        }
    }
}

//bresenham line drawing algorithm
void Renderer::plotLine(glm::vec2 p0, glm::vec2 p1, ogz_util::ColorRGB color)
{
    int dx = abs(p1.x - p0.x), sx = p0.x < p1.x ? 1 : -1;
    int dy = -abs(p1.y - p0.y), sy = p0.y < p1.y ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;)
    { /* loop */
        frame->setPixel(p0.x, p0.y, color);
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

void Renderer::viewportTransform(glm::vec3 &p, int viewport_width, int viewport_height)
{
    p.x = (int)((p.x + 1) / 2 * (viewport_width - 1));
    p.y = (int)((p.y + 1) / 2 * (viewport_height - 1));
}

float Renderer::calculatePixelDepth(const glm::vec3 bc_screen,
                          const glm::vec3 p0,
                          const glm::vec3 p1,
                          const glm::vec3 p2)
{
    float pixel_depth = 0;
    pixel_depth = p0.z * bc_screen.x +
                  p1.z * bc_screen.y +
                  p2.z * bc_screen.z;

    return pixel_depth;
}

glm::vec2 Renderer::calculatePixelTexCoord(const glm::vec3 bc_screen,
                                           const glm::vec2 t0,
                                           const glm::vec2 t1,
                                           const glm::vec2 t2)
{
    glm::vec2 texCoord;
    texCoord.x = t0.x * bc_screen.x +
                 t1.x * bc_screen.y +
                 t2.x * bc_screen.z;

    texCoord.y = t0.y * bc_screen.x +
                 t1.y * bc_screen.y +
                 t2.y * bc_screen.z;

    return texCoord;
}

void Renderer::drawTriangle(ogz_util::VertexData p0, ogz_util::VertexData p1, ogz_util::VertexData p2, Texture &texture)
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, -2.0));
    modelMatrix = glm::rotate(modelMatrix,glm::radians(rotateValue),glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 viewMatrix = camera->getCameraViewMatrix();
    glm::mat4 projectionMatrix = camera->getCameraProjectionMatrix();

    rendererShader.modelMatrix = modelMatrix;
    rendererShader.viewMatrix = viewMatrix;
    rendererShader.projectionMatrix = projectionMatrix;

    rendererShader.vertexShader(p0);
    rendererShader.vertexShader(p1);
    rendererShader.vertexShader(p2);

    viewportTransform(p0.vertex_pos, this->frame->getWidth(), this->frame->getHeight());
    viewportTransform(p1.vertex_pos, this->frame->getWidth(), this->frame->getHeight());
    viewportTransform(p2.vertex_pos, this->frame->getWidth(), this->frame->getHeight());

    glm::vec2 bboxmin(this->frame->getWidth() - 1, this->frame->getHeight() - 1);
    glm::vec2 bboxmax(0, 0);
    glm::vec2 clamp(this->frame->getWidth() - 1, this->frame->getHeight() - 1);

    bboxmin.x = std::max(0.0f, std::min(p0.vertex_pos.x, std::min(p1.vertex_pos.x, p2.vertex_pos.x)));
    bboxmin.y = std::max(0.0f, std::min(p0.vertex_pos.y, std::min(p1.vertex_pos.y, p2.vertex_pos.y)));

    bboxmax.x = std::min(clamp.x, std::max(p0.vertex_pos.x, std::max(p1.vertex_pos.x, p2.vertex_pos.x)));
    bboxmax.y = std::min(clamp.y, std::max(p0.vertex_pos.y, std::max(p1.vertex_pos.y, p2.vertex_pos.y)));

    glm::vec3 P;
    glm::vec2 pixelTexCoord;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            float pixel_depth_value = this->frame->getDepthBufferValue(P.x, P.y);
            glm::vec3 bc_screen = barycentric(P, p0.vertex_pos, p1.vertex_pos, p2.vertex_pos);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                continue;
            P.z = calculatePixelDepth(bc_screen, p0.vertex_pos, p1.vertex_pos, p2.vertex_pos);
            pixelTexCoord = calculatePixelTexCoord(bc_screen, p0.vertex_tex_coord, p1.vertex_tex_coord, p2.vertex_tex_coord);

            if (pixel_depth_value < P.z)
            {
                this->frame->setDepthBufferValue(P.x, P.y, P.z);
                this->frame->setPixel(P.x, P.y, texture.getColorValueByUV(pixelTexCoord.x, pixelTexCoord.y));
            }
        }
    }
}
