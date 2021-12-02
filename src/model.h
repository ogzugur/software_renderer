#pragma once
#include "tiny_obj_loader.h"
#include "image.h"
#include <string.h>

class Model
{
private:
    /* data */
    std::string inputfile;
    tinyobj::ObjReader reader;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    Image *frame;

    /* function */
    void plot_line(int x0, int y0, int x1, int y1, ColorRGB color);

public:
    Model();
    Model(std::string inputfile);
    ~Model();
    void draw(Image *frame, ColorRGB color);
};

void Model::draw(Image *frame, ColorRGB color)
{
    this->frame = frame;
    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++)
    {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            tinyobj::real_t triangle_vertices[6];
            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++)
            {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                triangle_vertices[v * 2 + 0] = int((vx + 1.0) * 399);
                triangle_vertices[v * 2 + 1] = int((vy + 1.0) * 399);

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0)
                {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0)
                {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                }
            }
            //draw triangle
            plot_line(triangle_vertices[0], triangle_vertices[1], triangle_vertices[2], triangle_vertices[3], color);
            plot_line(triangle_vertices[2], triangle_vertices[3], triangle_vertices[4], triangle_vertices[5], color);
            plot_line(triangle_vertices[4], triangle_vertices[5], triangle_vertices[0], triangle_vertices[1], color);

            index_offset += fv;
        }
    }
}

Model::Model()
{
}

Model::Model(std::string inputfile)
{
    if (!reader.ParseFromFile(inputfile))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }
    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    attrib = reader.GetAttrib();
    shapes = reader.GetShapes();
}

//bresenham line drawing algorithm
void Model::plot_line(int x0, int y0, int x1, int y1, ColorRGB color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;)
    { /* loop */
        this->frame->set_pixel(x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        } /* e_xy+e_x > 0 */
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        } /* e_xy+e_y < 0 */
    }
}
Model::~Model()
{
}
