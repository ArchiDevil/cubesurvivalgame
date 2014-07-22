#include "cBlkConverter.h"

#include <vector>
#include <cstdint>

#include "misc.h"

#include "../MathLib/math.h"

using namespace MathLib;

unsigned int GetIndex(unsigned x, unsigned y, unsigned z, unsigned x_size, unsigned y_size, unsigned z_size)
{
	return x * z_size * y_size + y * z_size + z;
}

struct Block
{
	Block()	
		: exist(false)
	{}

	Vector3F color;
	bool exist;
};

bool cBlkConverter::Convert( const std::wstring & in, const std::wstring & out )
{
	std::ifstream input;
	
#ifdef _MSC_VER
	input.open(in.c_str());
#else
	std::string str(in.begin(), in.end());
	input.open(str.c_str());
#endif

	if(input.fail() || !input.is_open())
		return false;

	uint32_t head[3] = {0};

	input.read((char*)head, sizeof(uint32_t) * 3);

	Block * dataArray = new Block[head[0] * head[1] * head[2]];

	for (unsigned i = 0; i < head[0] ; i++)
	{
		for (unsigned j = 0; j < head[1] ; j++)
		{
			for (unsigned k = 0; k < head[2] ; k++)
			{
				input.read((char*)(&dataArray[GetIndex(i, j, k, head[0], head[1], head[2])]), sizeof(Block));
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////

	std::vector<Vertex> vertices;
	std::vector<unsigned long> indices;
	unsigned int ind_index = 0;

	const Vector3F FRONT	= Vector3F(-1.0f, 0.0f, 0.0f);
	const Vector3F BACK		= Vector3F(1.0f, 0.0f, 0.0f);
	const Vector3F LEFT		= Vector3F(0.0f, -1.0f, 0.0f);
	const Vector3F RIGHT	= Vector3F(0.0f, 1.0f, 0.0f);
	const Vector3F DOWN		= Vector3F(0.0f, 0.0f, -1.0f);
	const Vector3F UP		= Vector3F(0.0f, 0.0f, 1.0f);

	for (float x = 0.0f; x < (float)head[0] ; x++)
	{
		for (float y = 0.0f; y < (float)head[1] ; y++)
		{
			for (float z = 0.0f; z < (float)head[2] ; z++)
			{
				auto block = dataArray[GetIndex(x, y, z, head[0], head[1], head[2])];
				if(block.exist)
				{
					//check around
					if(!dataArray[GetIndex(x - 1, y, z, head[0], head[1], head[2])].exist)
					{
						vertices.push_back(Vertex(Vector3F(x, y,	z),			FRONT, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x, y + 1,	z),		FRONT, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x, y,		z + 1),	FRONT, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x, y + 1,	z + 1),	FRONT, Vector2F()));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 1);
						indices.push_back(ind_index + 3);

						ind_index += 4;
					}					

					if(!dataArray[GetIndex(x + 1, y, z, head[0], head[1], head[2])].exist)
					{
						vertices.push_back(Vertex(Vector3F(x + 1, y,		z),		BACK, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x + 1, y + 1,	z),		BACK, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x + 1, y,		z + 1), BACK, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x + 1, y + 1,	z + 1), BACK, Vector2F()));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 1);

						ind_index += 4;
					}

					if(!dataArray[GetIndex(x, y - 1, z, head[0], head[1], head[2])].exist)
					{
						vertices.push_back(Vertex(Vector3F(x,		y,	z),		LEFT, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x + 1,	y,	z),		LEFT, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x,		y,	z + 1),	LEFT, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x + 1,	y,	z + 1),	LEFT, Vector2F()));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 1);

						ind_index += 4;
					}

					if(!dataArray[GetIndex(x, y + 1, z, head[0], head[1], head[2])].exist)
					{
						vertices.push_back(Vertex(Vector3F(x,		y + 1,	z),		RIGHT, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x + 1,	y + 1,	z),		RIGHT, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x,		y + 1,	z + 1),	RIGHT, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x + 1,	y + 1,	z + 1),	RIGHT, Vector2F()));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 1);
						indices.push_back(ind_index + 3);

						ind_index += 4;
					}

					if(!dataArray[GetIndex(x, y, z - 1, head[0], head[1], head[2])].exist)
					{
						vertices.push_back(Vertex(Vector3F(x,		y,		z),	DOWN, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x + 1,	y,		z),	DOWN, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x,		y + 1,	z),	DOWN, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x + 1,	y + 1,	z),	DOWN, Vector2F()));

						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 3);
						indices.push_back(ind_index + 2);
						indices.push_back(ind_index + 0);
						indices.push_back(ind_index + 1);
						indices.push_back(ind_index + 3);

						ind_index += 4;
					}

					if(!dataArray[GetIndex(x, y, z + 1, head[0], head[1], head[2])].exist)
					{
						vertices.push_back(Vertex(Vector3F(x,		y,		z + 1),	UP, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x + 1,	y,		z + 1),	UP, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x,		y + 1,	z + 1),	UP, Vector2F()));
						vertices.push_back(Vertex(Vector3F(x + 1,	y + 1,	z + 1),	UP, Vector2F()));

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

	MeshLIMHeader header;
	header.VERSION = LIM_HEADER_VERSION;
	header.hasNormals = true;
	header.hasTexCoords = false;
	header.verticesCount = (unsigned int)vertices.size();
	header.indicesCount = (unsigned int)indices.size();

	LIMSaver saver;
	bool result = saver.Save(out, vertices.data(), indices.data(), header);

	return result;
}
