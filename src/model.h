#pragma once
#include "frame.h"
#include "renderer.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader/tiny_obj_loader.h"
#include <string.h>
#include <glm/glm.hpp>

class Model
{
private:
    /* data */
    std::string inputfile;
    tinyobj::ObjReader reader;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;

public:
    Model();
    Model(std::string inputfile);
    ~Model();
    void draw(Frame &image);
};

void Model::draw(Frame &image)
{
    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++)
    {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            glm::vec3 triangle_vertices[3];
            ColorRGB color_normal = ColorRGB(0, 0, 0);
            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++)
            {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                //viewport transformation
                triangle_vertices[v] = glm::vec3(vx, vy, vz);

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0)
                {
                    tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    color_normal = ColorRGB((char)((nx + 1) / 2 * 255),
                                            (char)((ny + 1) / 2 * 255),
                                            (char)((nz + 1) / 2 * 255));
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0)
                {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                }
            }
            //draw triangle
            draw_triangle(triangle_vertices[0],
                          triangle_vertices[1],
                          triangle_vertices[2], image, color_normal);

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

Model::~Model()
{
}
