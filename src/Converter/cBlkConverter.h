#pragma once

#include "IConverter.h"

#include <memory>

class cBlkConverter : public IConverter
{
    struct Block
    {
        MathLib::Vector3F color = {};
        bool exist = false;
    };

public:
    cBlkConverter();
    virtual bool Convert(std::ifstream & in, std::vector<Vertex> & vertices, std::vector<uint32_t> & indices, MeshLIMHeader & header) override;

private:
    void SetSize(unsigned int x_size, unsigned int y_size, unsigned int z_size);
    void SetData(std::unique_ptr<Block[]> && data);

    const Block* GetBlock(unsigned x, unsigned y, unsigned z) const;
    unsigned int GetIndex(unsigned x, unsigned y, unsigned z) const;

    std::unique_ptr<Block[]> data = nullptr;
    unsigned int x_size = 0;
    unsigned int y_size = 0;
    unsigned int z_size = 0;
};
