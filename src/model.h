#pragma once
#include <string.h>
#include <vector>
#include "texture.h"
#include "mesh.h"
#include "tiny_obj_loader/tiny_obj_loader.h"

class Model
{
private:
    /* data */
    std::string directory;
    std::string inputfile;
    tinyobj::attrib_t attrib;
    tinyobj::ObjReader reader;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::vector<Texture> textures_loaded;

    /* function */
    void processModel();
    Mesh processMesh(tinyobj::shape_t shape);
    void calculateVertexTangentBitangent(ogz_util::VertexData *vertices);
    Texture load_material_textures(tinyobj::material_t mat, ogz_util::textureType type);

public:
    Model();
    Model(std::string inputfile);
    ~Model();
    Texture *texture;
    std::vector<Mesh> meshes;
};
