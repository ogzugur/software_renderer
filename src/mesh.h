#pragma once
#include <vector>
#include "texture.h"
#include "utility_structs.h"

class Mesh
{
public:
    Mesh();
    Mesh::Mesh(std::vector<ogz_util::VertexData> vertices,std::vector<Texture> textures);
    ~Mesh();

public:
    std::vector<ogz_util::VertexData> vertices;
    std::vector<Texture> textures;
    void Draw(size_t shader_id, bool b_load_textures);
};
