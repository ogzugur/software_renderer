#pragma once
#include <string.h>
#include "frame.h"
#include "texture.h"
#include "utility_structs.h"
#include "tiny_obj_loader/tiny_obj_loader.h"

class Model
{
private:
    /* data */
    std::string inputfile;
    tinyobj::ObjReader reader;

public:
    Model();
    Model(std::string inputfile);
    ~Model();
    Texture *texture;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
};

 