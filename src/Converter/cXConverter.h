#pragma once

#include "IConverter.h"
#include "misc.h"

class cXConverter : public IConverter
{
public:
    virtual bool Convert(std::ifstream & in, std::vector<Vertex> & vertices, std::vector<uint32_t> & indices, MeshLIMHeader & header) override;
};
