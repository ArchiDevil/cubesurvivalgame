#include "WorldTesselator.h"

#include "worldGenerator/cNoise.h"

#include <GraphicsEngine/ShiftEngine.h>

WorldTesselator::WorldTesselator( WorldStorage * _ws )
	: ws(_ws)
{
	auto pCtxMgr = ShiftEngine::GetContextManager();
	ShiftEngine::VertexSemantic semantic;
	semantic.addSemantic(ShiftEngine::ET_FLOAT, 3, ShiftEngine::ES_Position);
	semantic.addSemantic(ShiftEngine::ET_FLOAT, 3, ShiftEngine::ES_Normal);
	semantic.addSemantic(ShiftEngine::ET_FLOAT, 3, ShiftEngine::ES_Color);
	pCtxMgr->RegisterVertexSemantic(semantic);
	vd = pCtxMgr->GetVertexDeclaration(semantic);
}

WorldTesselator::~WorldTesselator()
{
}

bool WorldTesselator::TesselateChunk( int ChunkX, int ChunkY, ShiftEngine::MeshNode * node )
{
	struct PNC
	{
		PNC(const Vector3F & pos, const Vector3F & norm, const Vector3F & col)
			: Pos(pos), Normal(norm), Color(col) {}
		Vector3F Pos;
		Vector3F Normal;
		Vector3F Color;
	};

	static const Vector3F NormUP = Vector3F(0.0f, 0.0f, 1.0f);
	static const Vector3F NormXp = Vector3F(1.0f, 0.0f, 0.0f);
	static const Vector3F NormXm = Vector3F(-1.0f, 0.0f, 0.0f);
	static const Vector3F NormYp = Vector3F(0.0f, 1.0f, 0.0f);
	static const Vector3F NormYm = Vector3F(0.0f, -1.0f, 0.0f);

	static const Vector3F colors[] = 
	{
		Vector3F(1.0f, 1.0f, 1.0f),									//EMPTY COLOR
		Vector3F(231.0f / 255.0f, 208.0f / 255.0f, 82.0f / 255.0f),	//SAND COLOR
		Vector3F(107.0f / 255.0f, 95.0f / 255.0f, 28.0f / 255.0f),	//DIRT COLOR
		Vector3F(40.0f / 255.0f, 161.0f / 255.0f, 40.0f / 255.0f),	//GRASS COLOR
		Vector3F(99.0f / 255.0f, 99.0f / 255.0f, 99.0f / 255.0f),	//STONE COLOR
		Vector3F(118.0f / 255.0f, 212.0f / 255.0f, 247.0f / 255.0f) //WATER COLOR
	};

	const int chunkWidth = ws->GetChunkWidth();
	ShiftEngine::MeshData * data = new ShiftEngine::MeshData();
	int blockXstart = ChunkX * chunkWidth;
	int blockYstart = ChunkY * chunkWidth;

	std::vector<PNC> vertices;
	std::vector<unsigned long> indices;

	cNoise noiseGen;
	noiseGen.SetFrequency(0.01);
	//noiseGen.SetOctaves(5);

	for (int i = 0; i < chunkWidth; i++)
	{
		for (int j = 0; j < chunkWidth; j++)
		{
			float shift1 = (float)noiseGen.SimplexNoise((double)(i + blockXstart), (double)(j + blockYstart)) * 30.0f / 255.0f;
			float shift2 = (float)noiseGen.SimplexNoise((double)(i + blockXstart), (double)(j + blockYstart)) * 30.0f / 255.0f;
			float shift3 = (float)noiseGen.SimplexNoise((double)(i + blockXstart), (double)(j + blockYstart)) * 30.0f / 255.0f;
			Vector3F randVector = Vector3F(shift1, shift2, shift3);

			unsigned long curIndex = vertices.size();

			float height = (float)ws->GetFullHeight(blockXstart + i, blockYstart + j);

			//UP

			Vector3F color = colors[ws->GetBlockType(blockXstart + i, blockYstart + j, (unsigned int)height - 1)];
			unsigned int targetedHeight = (unsigned int)height;
			BlockTypes blocks[3][3];
			for (int w = -1; w <= 1; ++w)
				for (int h = -1; h <= 1; ++h)
					blocks[w + 1][h + 1] = ws->GetBlockType(blockXstart + i + w, blockYstart + j + h, targetedHeight);

			float colorModifier = 1.0f;
			if (blocks[0][1] != BT_Empty) colorModifier -= 0.25f;
			if (blocks[0][2] != BT_Empty) colorModifier -= 0.25f;
			if (blocks[1][2] != BT_Empty) colorModifier -= 0.25f;
			vertices.push_back(PNC(Vector3F(i + 0.0f, j + 1.0f, height), NormUP, (color + randVector) * colorModifier));

			colorModifier = 1.0f;
			if (blocks[2][2] != BT_Empty) colorModifier -= 0.25f;
			if (blocks[2][1] != BT_Empty) colorModifier -= 0.25f;
			if (blocks[1][2] != BT_Empty) colorModifier -= 0.25f;
			vertices.push_back(PNC(Vector3F(i + 1.0f, j + 1.0f, height), NormUP, (color + randVector) * colorModifier));

			colorModifier = 1.0f;
			if (blocks[1][0] != BT_Empty) colorModifier -= 0.25f;
			if (blocks[2][0] != BT_Empty) colorModifier -= 0.25f;
			if (blocks[2][1] != BT_Empty) colorModifier -= 0.25f;
			vertices.push_back(PNC(Vector3F(i + 1.0f, j + 0.0f, height), NormUP, (color + randVector) * colorModifier));

			colorModifier = 1.0f;
			if (blocks[0][0] != BT_Empty) colorModifier -= 0.25f;
			if (blocks[1][0] != BT_Empty) colorModifier -= 0.25f;
			if (blocks[0][1] != BT_Empty) colorModifier -= 0.25f;
			vertices.push_back(PNC(Vector3F(i + 0.0f, j + 0.0f, height), NormUP, (color + randVector) * colorModifier));

			indices.push_back(curIndex + 0);
			indices.push_back(curIndex + 1);
			indices.push_back(curIndex + 2);

			indices.push_back(curIndex + 0);
			indices.push_back(curIndex + 2);
			indices.push_back(curIndex + 3);

			//LEFT

			float neighboorHeight = (float)ws->GetFullHeight(blockXstart + i - 1, blockYstart + j);

			if(neighboorHeight < height)
			{
				for (int k = (int)neighboorHeight; k < (int)height; k++)
				{
					curIndex = vertices.size();

					color = colors[ws->GetBlockType(blockXstart + i, blockYstart + j, k)];
					vertices.push_back(PNC(Vector3F(i + 0.0f, j + 1.0f, k + 1.0f), NormXp, color));
					vertices.push_back(PNC(Vector3F(i + 0.0f, j + 0.0f, k + 1.0f), NormXp, color));
					vertices.push_back(PNC(Vector3F(i + 0.0f, j + 0.0f, k + 0.0f), NormXp, color));
					vertices.push_back(PNC(Vector3F(i + 0.0f, j + 1.0f, k + 0.0f), NormXp, color));

					indices.push_back(curIndex + 0);
					indices.push_back(curIndex + 1);
					indices.push_back(curIndex + 2);

					indices.push_back(curIndex + 0);
					indices.push_back(curIndex + 2);
					indices.push_back(curIndex + 3);
				}
			}

			//RIGHT

			neighboorHeight = (float)ws->GetFullHeight(blockXstart + i + 1, blockYstart + j);

			if(neighboorHeight < height)
			{
				for (int k = (int)neighboorHeight; k < (int)height; k++)
				{
					curIndex = vertices.size();

					color = colors[ws->GetBlockType(blockXstart + i, blockYstart + j, k)];
					vertices.push_back(PNC(Vector3F(i + 1.0f, j + 0.0f, k + 1.0f), NormXm, color));
					vertices.push_back(PNC(Vector3F(i + 1.0f, j + 1.0f, k + 1.0f), NormXm, color));
					vertices.push_back(PNC(Vector3F(i + 1.0f, j + 1.0f, k + 0.0f), NormXm, color));
					vertices.push_back(PNC(Vector3F(i + 1.0f, j + 0.0f, k + 0.0f), NormXm, color));

					indices.push_back(curIndex + 0);
					indices.push_back(curIndex + 1);
					indices.push_back(curIndex + 2);

					indices.push_back(curIndex + 0);
					indices.push_back(curIndex + 2);
					indices.push_back(curIndex + 3);
				}
			}

			//FRONT

			neighboorHeight = (float)ws->GetFullHeight(blockXstart + i, blockYstart + j - 1);

			if(neighboorHeight < height)
			{
				for (int k = (int)neighboorHeight; k < (int)height; k++)
				{
					curIndex = vertices.size();

					color = colors[ws->GetBlockType(blockXstart + i, blockYstart + j, k)];
					vertices.push_back(PNC(Vector3F(i + 0.0f, j + 0.0f, k + 1.0f), NormYm, color));
					vertices.push_back(PNC(Vector3F(i + 1.0f, j + 0.0f, k + 1.0f), NormYm, color));
					vertices.push_back(PNC(Vector3F(i + 1.0f, j + 0.0f, k + 0.0f), NormYm, color));
					vertices.push_back(PNC(Vector3F(i + 0.0f, j + 0.0f, k + 0.0f), NormYm, color));

					indices.push_back(curIndex + 0);
					indices.push_back(curIndex + 1);
					indices.push_back(curIndex + 2);

					indices.push_back(curIndex + 0);
					indices.push_back(curIndex + 2);
					indices.push_back(curIndex + 3);
				}
			}

			//BOTTOM

			neighboorHeight = (float)ws->GetFullHeight(blockXstart + i, blockYstart + j + 1);

			if(neighboorHeight < height)
			{
				for (int k = (int)neighboorHeight; k < (int)height; k++)
				{
					curIndex = vertices.size();

					color = colors[ws->GetBlockType(blockXstart + i, blockYstart + j, k)];
					vertices.push_back(PNC(Vector3F(i + 1.0f, j + 1.0f, k + 1.0f), NormYp, color));
					vertices.push_back(PNC(Vector3F(i + 0.0f, j + 1.0f, k + 1.0f), NormYp, color));
					vertices.push_back(PNC(Vector3F(i + 0.0f, j + 1.0f, k + 0.0f), NormYp, color));
					vertices.push_back(PNC(Vector3F(i + 1.0f, j + 1.0f, k + 0.0f), NormYp, color));

					indices.push_back(curIndex + 0);
					indices.push_back(curIndex + 1);
					indices.push_back(curIndex + 2);

					indices.push_back(curIndex + 0);
					indices.push_back(curIndex + 2);
					indices.push_back(curIndex + 3);
				}
			}
		}
	}

	if(!data->CreateBuffers(false, vertices.data(), sizeof(PNC) * vertices.size(), indices.data(), 
		sizeof(long) * indices.size(), ShiftEngine::GetContextManager()->GetDevicePointer()))
	{
		delete data;
		return false;
	}

	data->indicesCount = indices.size();
	data->verticesCount = vertices.size();
	data->vertexSize = sizeof(PNC);
	data->vertexDeclaration = vd;

	node->SetDataPtr(ShiftEngine::MeshDataPtr(data));

	return true;
}

void WorldTesselator::SetCriticalSection(std::mutex * sect)
{
	criticalSection = sect;
}
