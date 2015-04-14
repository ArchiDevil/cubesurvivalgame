#include "BlockStorage.h"

#include <cassert>
#include <algorithm>

MeshEditor::BlockStorage::BlockStorage(size_t x, size_t y, size_t z)
    : x_size(x)
    , y_size(y)
    , z_size(z)
{
    storage.reset(new Block[x * y * z]);
}

MeshEditor::Block & MeshEditor::BlockStorage::GetBlock(size_t x, size_t y, int z)
{
    if (x < x_size && y < y_size && (size_t)z < z_size && z >= 0)
    {
        return storage[x * z_size * y_size + y * z_size + z];
    }
    else if (z < 0)
    {
        static Block hack;
        hack.exist = true;
        return hack;
    }
    else
    {
        static Block local;
        return local;
    }
}

MeshEditor::Block MeshEditor::BlockStorage::GetBlock(size_t x, size_t y, int z) const
{
    if (x < x_size && y < y_size && (size_t)z < z_size && z >= 0)
    {
        return storage[x * z_size * y_size + y * z_size + z];
    }
    else if (z < 0)
    {
        Block hack;
        hack.exist = true;
        return hack;
    }
    else
    {
        return Block();
    }
}

void MeshEditor::BlockStorage::Resize(int Xup, int Yup, int Zup, int Xdown, int Ydown, int Zdown)
{
    assert((int)x_size + Xup + Xdown > 0);
    assert((int)y_size + Yup + Ydown > 0);
    assert((int)z_size + Zup + Zdown > 0);
    size_t x_new = x_size + Xup + Xdown;
    size_t y_new = y_size + Yup + Ydown;
    size_t z_new = z_size + Zup + Zdown;

    Block * ElementsNew = new Block[x_new * y_new * z_new];

    for (size_t i = 0; i < x_size; i++)
    {
        for (size_t j = 0; j < y_size; j++)
        {
            for (size_t k = 0; k < z_size; k++)
            {
                size_t indexnew = GetIndexNew(i + Xdown, j + Ydown, k + Zdown, x_new, y_new, z_new);
                if (GetBlock(i, j, k).exist)
                    ElementsNew[indexnew] = GetBlock(i, j, k);
            }
        }
    }

    storage.reset(ElementsNew);

    x_size = x_new;
    y_size = y_new;
    z_size = z_new;
}

void MeshEditor::BlockStorage::Resize(size_t x_size, size_t y_size, size_t z_size)
{
    this->x_size = x_size;
    this->y_size = y_size;
    this->z_size = z_size;

    storage.reset(new Block[x_size * y_size * z_size]);
}

void MeshEditor::BlockStorage::Clear()
{
    for (size_t i = 0; i < x_size * y_size * z_size; ++i)
    {
        storage[i].exist = false;
        storage[i].color = { 0.9f, 0.9f, 0.9f };
    }
}

void MeshEditor::BlockStorage::Fill(const MathLib::Vector3F& color)
{
    for (size_t i = 0; i < x_size * y_size * z_size; ++i)
    {
        storage[i].exist = true;
        storage[i].color = color;
    }
}

void MeshEditor::BlockStorage::Serialize(std::vector<uint8_t> & buffer)
{
    uint8_t * dataPtr = (uint8_t *)storage.get();
    buffer.assign(dataPtr, dataPtr + sizeof(Block) * x_size * y_size * z_size);
}

void MeshEditor::BlockStorage::Deserialize(const std::vector<uint8_t> & buffer)
{
    storage.reset(new Block[buffer.size() / sizeof(Block)]);
    uint8_t * dataPtr = (uint8_t*)storage.get();
    std::copy(buffer.begin(), buffer.end(), dataPtr);
}

MathLib::Vector3F MeshEditor::BlockStorage::GetHalfSize() const
{
    return{ (float)x_size / 2.0f, (float)y_size / 2.0f, (float)z_size / 2.0f };
}

int MeshEditor::BlockStorage::GetMaxSize() const
{
    return std::max(x_size, std::max(y_size, z_size));
}

int MeshEditor::BlockStorage::GetIndex(size_t x, size_t y, size_t z) const
{
    return x * z_size * y_size + y * z_size + z;
}

int MeshEditor::BlockStorage::GetIndexNew(size_t x, size_t y, size_t z, size_t /*xSize*/, size_t ySize, size_t zSize)
{
    return x * zSize * ySize + y * zSize + z;
}
