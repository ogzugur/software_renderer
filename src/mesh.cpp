#include "mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<ogz_util::VertexData> vertices,std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->textures = textures;
}

Mesh::~Mesh()
{

}