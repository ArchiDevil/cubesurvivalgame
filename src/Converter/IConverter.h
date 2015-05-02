#pragma once

#include <vector>

#include "LIMSaver.h"
#include "misc.h"

class IConverter
{
public:
    virtual ~IConverter() {}
    virtual bool Convert(std::ifstream & in, std::vector<Vertex> & vertices, std::vector<uint32_t> & indices, MeshLIMHeader & header) = 0;
};
