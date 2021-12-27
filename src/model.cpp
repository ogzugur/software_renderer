#include <glm/glm.hpp>
#include "model.h"
#include "renderer.h"

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
