#pragma once

#include "IConverter.h"

#include <memory>

class cBlkConverter : public IConverter
{
    struct Block
    {
        Block()
            : color()
            , exist(false)
        {}

        Vector3F color;
        bool exist;
    };

public:
    cBlkConverter();
    virtual bool Convert(const std::string & in, const std::string & out) override;

private:
    void SetSize(unsigned int x_size, unsigned int y_size, unsigned int z_size);
    void SetData(std::unique_ptr<Block[]> && data);

    const Block* GetBlock(unsigned x, unsigned y, unsigned z) const;
    unsigned int GetIndex(unsigned x, unsigned y, unsigned z) const;

    std::unique_ptr<Block[]> data;
    unsigned int x_size;
    unsigned int y_size;
    unsigned int z_size;
};
