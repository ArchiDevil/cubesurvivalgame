#pragma once

#include <Utilities/IManager.h>
#include <MathLib/math.h>

#include <cstdint>
#include <vector>
#include <memory>

namespace MeshEditor
{
    struct Block
    {
        Block()
            : exist(false)
        {
        }

        MathLib::Vector3F color;
        bool exist;
    };

    class BlockStorage final : public IManager
    {
    public:
        BlockStorage(size_t x, size_t y, size_t z);

        Block & GetBlock(size_t x, size_t y, int z);
        Block GetBlock(size_t x, size_t y, int z) const;

        void Resize(int Xup, int Yup, int Zup, int Xdown, int Ydown, int Zdown);
        void Resize(size_t x_size, size_t y_size, size_t z_size);

        void Clear();
        void Fill(const MathLib::Vector3F& color);

        void Serialize(std::vector<uint8_t> & buffer);
        void Deserialize(const std::vector<uint8_t> & buffer);

        MathLib::Vector3F GetHalfSize() const;
        int GetMaxSize() const;

    private:
        int GetIndex(size_t x, size_t y, size_t z) const;
        int GetIndexNew(size_t x, size_t y, size_t z, size_t xSize, size_t ySize, size_t zSize);

        std::unique_ptr<Block[]> storage;
        size_t x_size, y_size, z_size;
    };
}
