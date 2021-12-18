#pragma once
#include <algorithm>
#include <glm/glm.hpp>

void plot_line(glm::vec2 p0, glm::vec2 p1, ColorRGB color, Frame &frame);
glm::vec3 barycentric(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c);
void viewport_transform(glm::vec3 &p, int viewport_width, int viewport_height);
void draw_triangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, Frame &image, ColorRGB color);
float calculate_pixel_depth(const glm::vec3 bc_screen,
                            const glm::vec3 p0,
                            const glm::vec3 p1,
                            const glm::vec3 p2);


//bresenham line drawing algorithm
void plot_line(glm::vec2 p0, glm::vec2 p1, ColorRGB color, Frame &frame)
{
    int dx = abs(p1.x - p0.x), sx = p0.x < p1.x ? 1 : -1;
    int dy = -abs(p1.y - p0.y), sy = p0.y < p1.y ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;)
    { /* loop */
        frame.set_pixel(p0.x, p0.y, color);
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
glm::vec3 barycentric(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c)
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

void viewport_transform(glm::vec3 &p, int viewport_width, int viewport_height)
{
    p.x = (int)(((p.x + 1) / 2 * viewport_width) - 1);
    p.y = (int)(((-p.y + 1) / 2 * viewport_height) - 1);
}

float calculate_pixel_depth(const glm::vec3 bc_screen,
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


void draw_triangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, Frame &image, ColorRGB color)
{
    viewport_transform(p0, image.get_width(), image.get_height());
    viewport_transform(p1, image.get_width(), image.get_height());
    viewport_transform(p2, image.get_width(), image.get_height());

    glm::vec2 bboxmin(image.get_width() - 1, image.get_height() - 1);
    glm::vec2 bboxmax(0, 0);
    glm::vec2 clamp(image.get_width() - 1, image.get_height() - 1);

    bboxmin.x = std::min(p0.x, std::min(p1.x, p2.x));
    bboxmax.x = std::max(p0.x, std::max(p1.x, p2.x));

    bboxmin.y = std::min(p0.y, std::min(p1.y, p2.y));
    bboxmax.y = std::max(p0.y, std::max(p1.y, p2.y));

    glm::vec3 P;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            float pixel_depth_value = image.get_depth_buffer_value(P.x, P.y);
            glm::vec3 bc_screen = barycentric(P, p0, p1, p2);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                continue;
            P.z = calculate_pixel_depth(bc_screen, p0, p1, p2);

            if (pixel_depth_value < P.z)
            {
                image.set_dept_buffer_value(P.x, P.y, P.z);
                image.set_pixel(P.x, P.y, color);
            }
        }
    }
}


