#include "cBlkConverter.h"
#include "misc.h"

#include <vector>
#include <cstdint>

#include <MathLib/math.h>

using namespace MathLib;

cBlkConverter::cBlkConverter()
    : data(nullptr)
    , x_size(0)
    , y_size(0)
    , z_size(0)
{
}

bool cBlkConverter::Convert(const std::wstring & in, const std::wstring & out)
{
    std::ifstream input;

#ifdef _MSC_VER
    input.open(in.c_str());
#else
    std::string str(in.begin(), in.end());
    input.open(str.c_str());
#endif

    if (input.fail() || !input.is_open())
        return false;

    uint32_t head[3] = { 0 };
    input.read((char*)head, sizeof(uint32_t) * 3);
    SetSize(head[0], head[1], head[2]);

    data.reset(new Block[x_size * y_size * z_size]);
    for (unsigned i = 0; i < x_size; i++)
        for (unsigned j = 0; j < y_size; j++)
            for (unsigned k = 0; k < z_size; k++)
                input.read((char*)(&data[GetIndex(i, j, k)]), sizeof(Block));

    //////////////////////////////////////////////////////////////////////////

    std::vector<Vertex> vertices;
    std::vector<unsigned long> indices;
    unsigned int ind_index = 0;

    const Vector3F FRONT{ -1.0f, 0.0f, 0.0f };
    const Vector3F BACK{ 1.0f, 0.0f, 0.0f };
    const Vector3F LEFT{ 0.0f, -1.0f, 0.0f };
    const Vector3F RIGHT{ 0.0f, 1.0f, 0.0f };
    const Vector3F DOWN{ 0.0f, 0.0f, -1.0f };
    const Vector3F UP{ 0.0f, 0.0f, 1.0f };

    for (float x = 0.0f; x < (float)x_size; x += 1.0f)
    {
        for (float y = 0.0f; y < (float)y_size; y += 1.0f)
        {
            for (float z = 0.0f; z < (float)z_size; z += 1.0f)
            {
                auto block = GetBlock((unsigned)x, (unsigned)y, (unsigned)z);
                if (block->exist)
                {
                    auto buffer = GetBlock((unsigned)x - 1, (unsigned)y, (unsigned)z);
					if (!buffer->exist)
                    {
                        vertices.push_back(Vertex({ x, y, z }, FRONT, {}, block->color));
                        vertices.push_back(Vertex({ x, y + 1, z }, FRONT, {}, block->color));
                        vertices.push_back(Vertex({ x, y, z + 1 }, FRONT, {}, block->color));
                        vertices.push_back(Vertex({ x, y + 1, z + 1 }, FRONT, {}, block->color));

                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 2);
                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 1);
                        indices.push_back(ind_index + 3);

                        ind_index += 4;
                    }

					buffer = GetBlock((unsigned)x + 1, (unsigned)y, (unsigned)z);
					if (!buffer->exist)
                    {
                        vertices.push_back(Vertex({ x + 1, y, z }, BACK, {}, block->color));
                        vertices.push_back(Vertex({ x + 1, y + 1, z }, BACK, {}, block->color));
                        vertices.push_back(Vertex({ x + 1, y, z + 1 }, BACK, {}, block->color));
                        vertices.push_back(Vertex({ x + 1, y + 1, z + 1 }, BACK, {}, block->color));

                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 2);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 1);

                        ind_index += 4;
                    }

					buffer = GetBlock((unsigned)x, (unsigned)y - 1, (unsigned)z);
					if (!buffer->exist)
                    {
                        vertices.push_back(Vertex({ x, y, z }, LEFT, {}, block->color));
                        vertices.push_back(Vertex({ x + 1, y, z }, LEFT, {}, block->color));
                        vertices.push_back(Vertex({ x, y, z + 1 }, LEFT, {}, block->color));
                        vertices.push_back(Vertex({ x + 1, y, z + 1 }, LEFT, {}, block->color));

                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 2);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 1);

                        ind_index += 4;
                    }

					buffer = GetBlock((unsigned)x, (unsigned)y + 1, (unsigned)z);
					if (!buffer->exist)
                    {
                        vertices.push_back(Vertex({ x, y + 1, z }, RIGHT, {}, block->color));
                        vertices.push_back(Vertex({ x + 1, y + 1, z }, RIGHT, {}, block->color));
                        vertices.push_back(Vertex({ x, y + 1, z + 1 }, RIGHT, {}, block->color));
                        vertices.push_back(Vertex({ x + 1, y + 1, z + 1 }, RIGHT, {}, block->color));

                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 2);
                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 1);
                        indices.push_back(ind_index + 3);

                        ind_index += 4;
                    }

					buffer = GetBlock((unsigned)x, (unsigned)y, (unsigned)z - 1);
					if (!buffer->exist)
                    {
                        vertices.push_back(Vertex({ x, y, z }, DOWN, {}, block->color));
                        vertices.push_back(Vertex({ x + 1, y, z }, DOWN, {}, block->color));
                        vertices.push_back(Vertex({ x, y + 1, z }, DOWN, {}, block->color));
                        vertices.push_back(Vertex({ x + 1, y + 1, z }, DOWN, {}, block->color));

                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 2);
                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 1);
                        indices.push_back(ind_index + 3);

                        ind_index += 4;
                    }

					buffer = GetBlock((unsigned)x, (unsigned)y, (unsigned)z + 1);
					if (!buffer->exist)
                    {
                        vertices.push_back(Vertex({ x, y, z + 1 }, UP, {}, block->color));
                        vertices.push_back(Vertex({ x + 1, y, z + 1 }, UP, {}, block->color));
                        vertices.push_back(Vertex({ x, y + 1, z + 1 }, UP, {}, block->color));
                        vertices.push_back(Vertex({ x + 1, y + 1, z + 1 }, UP, {}, block->color));

                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 2);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 1);

                        ind_index += 4;
                    }
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // converting vertices into default loading file format

    for (size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i].Pos.x *= -1;
        std::swap(vertices[i].Pos.y, vertices[i].Pos.z);

        vertices[i].Normal.x *= -1;
        std::swap(vertices[i].Normal.y, vertices[i].Normal.z);
    }

    for (size_t i = 0; i < indices.size(); i += 3)
        std::swap(indices[i], indices[i + 1]);

    //////////////////////////////////////////////////////////////////////////

    MeshLIMHeader header;
    header.version = LIM_HEADER_VERSION;
    header.hasNormals = true;
    header.hasTexCoords = false;
	header.hasColors = true;
    header.verticesCount = (unsigned int)vertices.size();
    header.indicesCount = (unsigned int)indices.size();
    return LIMSaver::Save(out, vertices.data(), indices.data(), header);
}

void cBlkConverter::SetSize(unsigned int x_size, unsigned int y_size, unsigned int z_size)
{
    this->x_size = x_size;
    this->y_size = y_size;
    this->z_size = z_size;
}

void cBlkConverter::SetData(std::unique_ptr<Block[]> && data)
{
    this->data = std::move(data);
}

const cBlkConverter::Block* cBlkConverter::GetBlock(unsigned x, unsigned y, unsigned z) const
{
    static const Block stub;
    unsigned index = GetIndex(x, y, z);
    if (index == (unsigned)-1)
        return &stub;
    return &(data[index]);
}

unsigned int cBlkConverter::GetIndex(unsigned x, unsigned y, unsigned z) const
{
    if (x >= x_size || y >= y_size || z >= z_size)
        return (unsigned)-1;
    return x * z_size * y_size + y * z_size + z;
}
