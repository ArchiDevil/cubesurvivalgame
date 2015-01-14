#include "cBlkConverter.h"
#include "misc.h"

#include <vector>
#include <cstdint>

#include <MathLib/math.h>

using namespace MathLib;

unsigned int GetIndex(unsigned x, unsigned y, unsigned z, unsigned x_size, unsigned y_size, unsigned z_size)
{
	return x * z_size * y_size + y * z_size + z;
}

struct Block
{
	Block()
		: exist(false)
		, color()
	{}

	Vector3F color;
	bool exist;
};

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

	Block * dataArray = new Block[head[0] * head[1] * head[2]];
	for (unsigned i = 0; i < head[0]; i++)
		for (unsigned j = 0; j < head[1]; j++)
			for (unsigned k = 0; k < head[2]; k++)
				input.read((char*)(&dataArray[GetIndex(i, j, k, head[0], head[1], head[2])]), sizeof(Block));

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

	for (float x = 0.0f; x < (float)head[0]; x += 1.0f)
	{
		for (float y = 0.0f; y < (float)head[1]; y += 1.0f)
		{
			for (float z = 0.0f; z < (float)head[2]; z += 1.0f)
			{
				auto &block = dataArray[GetIndex(x, y, z, head[0], head[1], head[2])];
				if (block.exist)
				{
					if (!dataArray[GetIndex(x - 1, y, z, head[0], head[1], head[2])].exist)
					{
                        vertices.push_back(Vertex({ x, y, z }, FRONT, {}, block.color));
                        vertices.push_back(Vertex({ x, y + 1, z }, FRONT, {}, block.color));
                        vertices.push_back(Vertex({ x, y, z + 1 }, FRONT, {}, block.color));
                        vertices.push_back(Vertex({ x, y + 1, z + 1 }, FRONT, {}, block.color));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 1);
						indices.push_back(ind_index + 3);

						ind_index += 4;
					}

					if (!dataArray[GetIndex(x + 1, y, z, head[0], head[1], head[2])].exist)
					{
                        vertices.push_back(Vertex({ x + 1, y, z }, BACK, {}, block.color));
                        vertices.push_back(Vertex({ x + 1, y + 1, z }, BACK, {}, block.color));
                        vertices.push_back(Vertex({ x + 1, y, z + 1 }, BACK, {}, block.color));
                        vertices.push_back(Vertex({ x + 1, y + 1, z + 1 }, BACK, {}, block.color));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 1);

						ind_index += 4;
					}

					if (!dataArray[GetIndex(x, y - 1, z, head[0], head[1], head[2])].exist)
					{
                        vertices.push_back(Vertex({ x, y, z }, LEFT, {}, block.color));
                        vertices.push_back(Vertex({ x + 1, y, z }, LEFT, {}, block.color));
                        vertices.push_back(Vertex({ x, y, z + 1 }, LEFT, {}, block.color));
                        vertices.push_back(Vertex({ x + 1, y, z + 1 }, LEFT, {}, block.color));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 1);

						ind_index += 4;
					}

					if (!dataArray[GetIndex(x, y + 1, z, head[0], head[1], head[2])].exist)
					{
                        vertices.push_back(Vertex({ x, y + 1, z }, RIGHT, {}, block.color));
                        vertices.push_back(Vertex({ x + 1, y + 1, z }, RIGHT, {}, block.color));
                        vertices.push_back(Vertex({ x, y + 1, z + 1 }, RIGHT, {}, block.color));
                        vertices.push_back(Vertex({ x + 1, y + 1, z + 1 }, RIGHT, {}, block.color));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 1);
						indices.push_back(ind_index + 3);

						ind_index += 4;
					}

					if (!dataArray[GetIndex(x, y, z - 1, head[0], head[1], head[2])].exist)
					{
                        vertices.push_back(Vertex({ x, y, z }, DOWN, {}, block.color));
                        vertices.push_back(Vertex({ x + 1, y, z }, DOWN, {}, block.color));
                        vertices.push_back(Vertex({ x, y + 1, z }, DOWN, {}, block.color));
                        vertices.push_back(Vertex({ x + 1, y + 1, z }, DOWN, {}, block.color));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 1);
						indices.push_back(ind_index + 3);

						ind_index += 4;
					}

					if (!dataArray[GetIndex(x, y, z + 1, head[0], head[1], head[2])].exist)
					{
                        vertices.push_back(Vertex({ x, y, z + 1 }, UP, {}, block.color));
                        vertices.push_back(Vertex({ x + 1, y, z + 1 }, UP, {}, block.color));
                        vertices.push_back(Vertex({ x, y + 1, z + 1 }, UP, {}, block.color));
                        vertices.push_back(Vertex({ x + 1, y + 1, z + 1 }, UP, {}, block.color));

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
        vertices[i].Pos.x *= -1;					//X * -1
        std::swap(vertices[i].Pos.y, vertices[i].Pos.z);

        vertices[i].Normal.x *= -1;					//nX * -1
        std::swap(vertices[i].Normal.y, vertices[i].Normal.z);
    }

    for (size_t i = 0; i < indices.size(); i += 3)
        std::swap(indices[i], indices[i + 1]);

	//////////////////////////////////////////////////////////////////////////

	MeshLIMHeader header;
	header.version = LIM_HEADER_VERSION;
	header.hasNormals = true;
	header.hasTexCoords = false;
	header.verticesCount = (unsigned int)vertices.size();
	header.indicesCount = (unsigned int)indices.size();
	return LIMSaver::Save(out, vertices.data(), indices.data(), header);
}
