#include <glm/glm.hpp>
#include "model.h"
#include <iostream>

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

    directory = inputfile.substr(0, inputfile.find_last_of('/'));
    attrib = reader.GetAttrib();
    shapes = reader.GetShapes();
    materials = reader.GetMaterials();

    processModel();
}

Mesh Model::processMesh(tinyobj::shape_t shape)
{
    std::vector<ogz_util::VertexData> vertices;
    std::vector<Texture> textures;
    size_t index_offset = 0;
    for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
    {
        size_t fv = size_t(shape.mesh.num_face_vertices[f]);
        ogz_util::VertexData tempVertex[3];
        // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++)
        {
            // access to vertex
            tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
            tinyobj::real_t vx = this->attrib.vertices[3 * size_t(idx.vertex_index) + 0];
            tinyobj::real_t vy = this->attrib.vertices[3 * size_t(idx.vertex_index) + 1];
            tinyobj::real_t vz = this->attrib.vertices[3 * size_t(idx.vertex_index) + 2];

            tempVertex[v].vertex_pos = glm::vec3(vx, vy, vz);

            // Check if `normal_index` is zero or positive. negative = no normal data
            if (idx.normal_index >= 0)
            {
                tinyobj::real_t nx = this->attrib.normals[3 * size_t(idx.normal_index) + 0];
                tinyobj::real_t ny = this->attrib.normals[3 * size_t(idx.normal_index) + 1];
                tinyobj::real_t nz = this->attrib.normals[3 * size_t(idx.normal_index) + 2];

                tempVertex[v].vertex_normal = glm::vec3(nx, ny, nz);
            }

            // Check if `texcoord_index` is zero or positive. negative = no texcoord data
            if (idx.texcoord_index >= 0)
            {
                tinyobj::real_t tx = this->attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                tinyobj::real_t ty = this->attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                tempVertex[v].vertex_tex_coord = glm::vec2(tx, ty);
            }
            vertices.push_back(tempVertex[v]);
        }
        calculateVertexTangentBitangent(tempVertex);
        index_offset += fv;
    }
    textures.push_back(load_material_textures(materials[shape.mesh.material_ids[0]], ogz_util::TEXTURE_TYPE_DIFFUSE));
    textures.push_back(load_material_textures(materials[shape.mesh.material_ids[0]], ogz_util::TEXTURE_TYPE_NORMAL));

    return Mesh(vertices, textures);
}

void Model::calculateVertexTangentBitangent(ogz_util::VertexData *vertices)
{
    glm::vec3 edge1 = vertices[1].vertex_pos - vertices[0].vertex_pos;
    glm::vec3 edge2 = vertices[2].vertex_pos - vertices[0].vertex_pos;

    float deltaU1 = vertices[1].vertex_tex_coord.x - vertices[0].vertex_tex_coord.x;
    float deltaV1 = vertices[1].vertex_tex_coord.y - vertices[0].vertex_tex_coord.y;
    float deltaU2 = vertices[2].vertex_tex_coord.x - vertices[0].vertex_tex_coord.x;
    float deltaV2 = vertices[2].vertex_tex_coord.y - vertices[0].vertex_tex_coord.y;

    float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

    glm::vec3 tangent, bitangent;

    tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
    tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
    tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

    bitangent.x = f * (-deltaU2 * edge1.x + deltaU1 * edge2.x);
    bitangent.x = f * (-deltaU2 * edge1.y + deltaU1 * edge2.y);
    bitangent.x = f * (-deltaU2 * edge1.z + deltaU1 * edge2.z);

    for (size_t i = 0; i < 3; i++)
    {
        vertices[i].vertex_tangent = tangent;
        vertices[i].vertex_bitangent = bitangent;
    }
}

void Model::processModel()
{
    for (size_t s = 0; s < this->shapes.size(); s++)
    {
        tinyobj::shape_t currentShape = shapes[s];
        meshes.push_back(processMesh(currentShape));
    }
}

Texture Model::load_material_textures(tinyobj::material_t mat, ogz_util::textureType type)
{
    Texture texture;
    std::string texturePath;

    switch (type)
    {
    case ogz_util::TEXTURE_TYPE_ALPHA:
        texturePath = mat.alpha_texname;
        break;
    case ogz_util::TEXTURE_TYPE_AMBIENT:
        texturePath = mat.ambient_texname;
        break;
    case ogz_util::TEXTURE_TYPE_BUMP:
        texturePath = mat.bump_texname;
        break;
    case ogz_util::TEXTURE_TYPE_DIFFUSE:
        texturePath = mat.diffuse_texname;
        break;
    case ogz_util::TEXTURE_TYPE_DISPLACEMENT:
        texturePath = mat.displacement_texname;
        break;
    case ogz_util::TEXTURE_TYPE_EMISSIVE:
        texturePath = mat.emissive_texname;
        break;
    case ogz_util::TEXTURE_TYPE_METALLIC:
        texturePath = mat.metallic_texname;
        break;
    case ogz_util::TEXTURE_TYPE_NORMAL:
        texturePath = mat.normal_texname;
        break;
    case ogz_util::TEXTURE_TYPE_ROUGHNESS:
        texturePath = mat.roughness_texname;
        break;
    case ogz_util::TEXTURE_TYPE_SPECULAR:
        texturePath = mat.specular_texname;
        break;
    default:
        texturePath = " ";
        break;
    }

    bool skip = false;
    for (unsigned int j = 0; j < textures_loaded.size(); j++)
    {
        if (std::strcmp(textures_loaded[j].path.data(), texturePath.c_str()) == 0)
        {
            texture = textures_loaded[j];
            skip = true;
            break;
        }
    }
    if (!skip)
    {
        texture = Texture((directory + '/' + texturePath).c_str());
        texture.type = type;
        texture.path = texturePath;
        textures_loaded.push_back(texture);
    }
    return texture;
}

Model::~Model()
{
}
