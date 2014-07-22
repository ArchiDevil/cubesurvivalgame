#include "Tesselator.h"

#include "../../GraphicsEngine/ShiftEngine.h"

void cTesselator::Initialize( cWorldStorage * ws, cChunksStorage * Chunks, cWorldGenerator * gen )
{
	pStorage = ws;
	pChunks = Chunks;
	generator = gen;

	maxVertices = ws->GetChunkWidth() / 2 * ws->GetChunkWidth() / 2 * ws->GetChunkHeight() / 2 * 24;
	vertexPool = new LandVertex[maxVertices];
	indexPool = new long[maxVertices * 3 / 2];

	std::vector<std::wstring> grass;
	grass.push_back(L"resources/models/world/grass1.lim");
	grass.push_back(L"resources/models/world/grass2.lim");
	grass.push_back(L"resources/models/world/grass3.lim");
	LoadGrassMeshes(grass);
}

bool cTesselator::BuildChunkMesh( int WorldX, int WorldY )
{
	int blockLeftX = WorldX * pStorage->GetChunkWidth();
	int blockUpX = WorldY * pStorage->GetChunkWidth();
	WorldChunk * thisChunk = pChunks->GetChunkPtr(WorldX, WorldY);
	Block * bufferBlock;
	static unsigned int ChunkHeight = pStorage->GetChunkHeight();
	static unsigned int ChunkWidth = pStorage->GetChunkWidth();

	unsigned int verticesCount = 0;

	for (int i = blockLeftX; i < blockLeftX + (int)ChunkWidth; i++)
	{
		for (int j = blockUpX; j < blockUpX + (int)ChunkWidth; j++)
		{
			int blockIndex = pStorage->GetIndex(i, j, 0);
			for(unsigned int k = 0; k < ChunkHeight; k++)
			{
				bufferBlock = pStorage->GetBlock(blockIndex + k);
				if (bufferBlock->TypeID != ID_AIR && bufferBlock->TypeID != ID_WATER)
				{
					Block * block = nullptr;

					if(k < ChunkHeight)
					{
						block = pStorage->GetBlock(i, j, k+1);
						if(block->TypeID == ID_AIR || block->TypeID == ID_WATER)
						{
							AddToVertices(i+1 - blockLeftX,	j+1 - blockUpX,	k+1,	0.0f, 0.0f, bufferBlock->TypeID - 1, 0.0f, 0.0f, 1.0f, 
								(	pStorage->GetBlock(i,		j,		k + 1)->LightValue + 
								pStorage->GetBlock(i + 1,	j,		k + 1)->LightValue + 
								pStorage->GetBlock(i,		j + 1,	k + 1)->LightValue + 
								pStorage->GetBlock(i + 1,	j + 1,	k + 1)->LightValue) * 0.25f, vertexPool++);

							AddToVertices(i+1 - blockLeftX,	j - blockUpX,		k+1,	1.0f, 0.0f, bufferBlock->TypeID - 1, 0.0f, 0.0f, 1.0f, 
								(	pStorage->GetBlock(i,		j,		k + 1)->LightValue + 
								pStorage->GetBlock(i + 1,	j,		k + 1)->LightValue + 
								pStorage->GetBlock(i + 1,	j - 1,	k + 1)->LightValue + 
								pStorage->GetBlock(i,		j - 1,	k + 1)->LightValue) * 0.25f, vertexPool++);

							AddToVertices(i - blockLeftX,	j+1 - blockUpX,	k+1,	0.0f, 1.0f, bufferBlock->TypeID - 1, 0.0f, 0.0f, 1.0f, 
								(	pStorage->GetBlock(i,		j,		k + 1)->LightValue + 
								pStorage->GetBlock(i,		j + 1,	k + 1)->LightValue + 
								pStorage->GetBlock(i - 1,	j + 1,	k + 1)->LightValue + 
								pStorage->GetBlock(i - 1,	j,		k + 1)->LightValue) * 0.25f, vertexPool++);

							AddToVertices(i - blockLeftX,	j - blockUpX,		k+1,	1.0f, 1.0f, bufferBlock->TypeID - 1, 0.0f, 0.0f, 1.0f, 
								(	pStorage->GetBlock(i,		j,		k + 1)->LightValue + 
								pStorage->GetBlock(i - 1,	j,		k + 1)->LightValue + 
								pStorage->GetBlock(i,		j - 1,	k + 1)->LightValue + 
								pStorage->GetBlock(i - 1,	j - 1,	k + 1)->LightValue) * 0.25f, vertexPool++);

							*indexPool++ = verticesCount;
							*indexPool++ = verticesCount + 1;
							*indexPool++ = verticesCount + 2;
							*indexPool++ = verticesCount + 3;
							*indexPool++ = verticesCount + 2;
							*indexPool++ = verticesCount + 1;

							verticesCount += 4;
						}
					}

					if(k > 0)
					{
						block = pStorage->GetBlock(i, j, k-1);
						if(block->TypeID == ID_AIR || block->TypeID == ID_WATER)
						{
							AddToVertices(i - blockLeftX,	j+1 - blockUpX,	k,	0.0f, 0.0f, bufferBlock->TypeID - 1, 0.0f, 0.0f, -1.0f, 
								(	pStorage->GetBlock(i,		j,		k - 1)->LightValue + 
								pStorage->GetBlock(i,		j + 1,	k - 1)->LightValue + 
								pStorage->GetBlock(i - 1,	j + 1,	k - 1)->LightValue + 
								pStorage->GetBlock(i - 1,	j,		k - 1)->LightValue) * 0.25f, vertexPool++);

							AddToVertices(i - blockLeftX,	j - blockUpX,		k,	1.0f, 0.0f, bufferBlock->TypeID - 1, 0.0f, 0.0f, -1.0f, 
								(	pStorage->GetBlock(i,		j,		k - 1)->LightValue + 
								pStorage->GetBlock(i - 1,	j,		k - 1)->LightValue + 
								pStorage->GetBlock(i,		j - 1,	k - 1)->LightValue + 
								pStorage->GetBlock(i - 1,	j - 1,	k - 1)->LightValue) * 0.25f, vertexPool++);

							AddToVertices(i+1 - blockLeftX,	j+1 - blockUpX,	k,	0.0f, 1.0f, bufferBlock->TypeID - 1, 0.0f, 0.0f, -1.0f, 
								(	pStorage->GetBlock(i,		j,		k - 1)->LightValue + 
								pStorage->GetBlock(i + 1,	j,		k - 1)->LightValue + 
								pStorage->GetBlock(i,		j + 1,	k - 1)->LightValue + 
								pStorage->GetBlock(i + 1,	j + 1,	k - 1)->LightValue) * 0.25f, vertexPool++);

							AddToVertices(i+1 - blockLeftX,	j - blockUpX,		k,	1.0f, 1.0f, bufferBlock->TypeID - 1, 0.0f, 0.0f, -1.0f, 
								(	pStorage->GetBlock(i,		j,		k - 1)->LightValue + 
								pStorage->GetBlock(i + 1,	j,		k - 1)->LightValue + 
								pStorage->GetBlock(i + 1,	j - 1,	k - 1)->LightValue + 
								pStorage->GetBlock(i,		j - 1,	k - 1)->LightValue) * 0.25f, vertexPool++);

							*indexPool++ = verticesCount;
							*indexPool++ = verticesCount + 1;
							*indexPool++ = verticesCount + 2;
							*indexPool++ = verticesCount + 3;
							*indexPool++ = verticesCount + 2;
							*indexPool++ = verticesCount + 1;

							verticesCount += 4;
						}
					}

					block = pStorage->GetBlock(i+1, j, k);
					if(block->TypeID == ID_AIR || block->TypeID == ID_WATER)
					{
						AddToVertices(i+1 - blockLeftX,	j - blockUpX,		k+1,	0.0f, 0.0f, bufferBlock->TypeID - 1, 1.0f, 0.0f, 0.0f, 
							(	pStorage->GetBlock(i + 1, j, k)->LightValue + 
							pStorage->GetBlock(i + 1, j, k + 1)->LightValue + 
							pStorage->GetBlock(i + 1, j - 1, k)->LightValue + 
							pStorage->GetBlock(i + 1, j - 1, k + 1)->LightValue) * 0.25f, vertexPool++);

						AddToVertices(i+1 - blockLeftX,	j+1 - blockUpX,	k+1,	1.0f, 0.0f, bufferBlock->TypeID - 1, 1.0f, 0.0f, 0.0f, 
							(	pStorage->GetBlock(i + 1, j, k)->LightValue + 
							pStorage->GetBlock(i + 1, j, k + 1)->LightValue + 
							pStorage->GetBlock(i + 1, j + 1, k)->LightValue + 
							pStorage->GetBlock(i + 1, j + 1, k + 1)->LightValue) * 0.25f, vertexPool++);

						AddToVertices(i+1 - blockLeftX,	j - blockUpX,		k,		0.0f, 1.0f, bufferBlock->TypeID - 1, 1.0f, 0.0f, 0.0f, 
							(	pStorage->GetBlock(i + 1, j, k)->LightValue + 
							pStorage->GetBlock(i + 1, j, k - 1)->LightValue + 
							pStorage->GetBlock(i + 1, j - 1, k)->LightValue + 
							pStorage->GetBlock(i + 1, j - 1, k - 1)->LightValue) * 0.25f, vertexPool++);

						AddToVertices(i+1 - blockLeftX,	j+1 - blockUpX,	k,		1.0f, 1.0f, bufferBlock->TypeID - 1, 1.0f, 0.0f, 0.0f, 
							(	pStorage->GetBlock(i + 1, j, k)->LightValue + 
							pStorage->GetBlock(i + 1, j, k - 1)->LightValue + 
							pStorage->GetBlock(i + 1, j + 1, k)->LightValue + 
							pStorage->GetBlock(i + 1, j + 1, k - 1)->LightValue) * 0.25f, vertexPool++);

						*indexPool++ = verticesCount;
						*indexPool++ = verticesCount + 1;
						*indexPool++ = verticesCount + 2;
						*indexPool++ = verticesCount + 3;
						*indexPool++ = verticesCount + 2;
						*indexPool++ = verticesCount + 1;

						verticesCount += 4;
					}

					block = pStorage->GetBlock(i-1, j, k);
					if(block->TypeID == ID_AIR || block->TypeID == ID_WATER)
					{
						AddToVertices(i - blockLeftX,	j+1 - blockUpX,	k+1,	0.0f, 0.0f, bufferBlock->TypeID - 1, -1.0f, 0.0f, 0.0f, 
							(	pStorage->GetBlock(i - 1, j, k)->LightValue + 
							pStorage->GetBlock(i - 1, j, k + 1)->LightValue + 
							pStorage->GetBlock(i - 1, j + 1, k)->LightValue + 
							pStorage->GetBlock(i - 1, j + 1, k + 1)->LightValue) * 0.25f, vertexPool++);

						AddToVertices(i - blockLeftX,	j - blockUpX,		k+1,	1.0f, 0.0f, bufferBlock->TypeID - 1, -1.0f, 0.0f, 0.0f, 
							(	pStorage->GetBlock(i - 1, j, k)->LightValue + 
							pStorage->GetBlock(i - 1, j, k + 1)->LightValue + 
							pStorage->GetBlock(i - 1, j - 1, k)->LightValue + 
							pStorage->GetBlock(i - 1, j - 1, k + 1)->LightValue) * 0.25f, vertexPool++);

						AddToVertices(i - blockLeftX,	j+1 - blockUpX,	k,		0.0f, 1.0f, bufferBlock->TypeID - 1, -1.0f, 0.0f, 0.0f, 
							(	pStorage->GetBlock(i - 1, j, k)->LightValue + 
							pStorage->GetBlock(i - 1, j, k - 1)->LightValue + 
							pStorage->GetBlock(i - 1, j + 1, k)->LightValue + 
							pStorage->GetBlock(i - 1, j + 1, k - 1)->LightValue) * 0.25f, vertexPool++);

						AddToVertices(i - blockLeftX,	j - blockUpX,		k,		1.0f, 1.0f, bufferBlock->TypeID - 1, -1.0f, 0.0f, 0.0f, 
							(	pStorage->GetBlock(i - 1, j, k)->LightValue + 
							pStorage->GetBlock(i - 1, j, k - 1)->LightValue + 
							pStorage->GetBlock(i - 1, j - 1, k)->LightValue + 
							pStorage->GetBlock(i - 1, j - 1, k - 1)->LightValue) * 0.25f, vertexPool++);

						*indexPool++ = verticesCount;
						*indexPool++ = verticesCount + 1;
						*indexPool++ = verticesCount + 2;
						*indexPool++ = verticesCount + 3;
						*indexPool++ = verticesCount + 2;
						*indexPool++ = verticesCount + 1;

						verticesCount += 4;
					}

					block = pStorage->GetBlock(i, j+1, k);
					if(block->TypeID == ID_AIR || block->TypeID == ID_WATER)
					{
						AddToVertices(i+1 - blockLeftX,	j+1 - blockUpX,	k+1,	0.0f, 0.0f, bufferBlock->TypeID - 1, 0.0f, 1.0f, 0.0f, 
							(	pStorage->GetBlock(i, j + 1, k)->LightValue + 
							pStorage->GetBlock(i, j + 1, k + 1)->LightValue + 
							pStorage->GetBlock(i + 1, j + 1, k)->LightValue + 
							pStorage->GetBlock(i + 1, j + 1, k + 1)->LightValue) * 0.25f, vertexPool++);

						AddToVertices(i - blockLeftX,	j+1 - blockUpX,	k+1,	1.0f, 0.0f, bufferBlock->TypeID - 1, 0.0f, 1.0f, 0.0f, 
							(	pStorage->GetBlock(i, j + 1, k)->LightValue + 
							pStorage->GetBlock(i, j + 1, k + 1)->LightValue + 
							pStorage->GetBlock(i - 1, j + 1, k)->LightValue + 
							pStorage->GetBlock(i - 1, j + 1, k + 1)->LightValue) * 0.25f, vertexPool++);

						AddToVertices(i+1 - blockLeftX,	j+1 - blockUpX,	k,		0.0f, 1.0f, bufferBlock->TypeID - 1, 0.0f, 1.0f, 0.0f, 
							(	pStorage->GetBlock(i, j + 1, k)->LightValue + 
							pStorage->GetBlock(i, j + 1, k - 1)->LightValue + 
							pStorage->GetBlock(i + 1, j + 1, k)->LightValue + 
							pStorage->GetBlock(i + 1, j + 1, k - 1)->LightValue) * 0.25f, vertexPool++);

						AddToVertices(i - blockLeftX,	j+1 - blockUpX,	k,		1.0f, 1.0f, bufferBlock->TypeID - 1, 0.0f, 1.0f, 0.0f, 
							(	pStorage->GetBlock(i, j + 1, k)->LightValue + 
							pStorage->GetBlock(i, j + 1, k - 1)->LightValue + 
							pStorage->GetBlock(i - 1, j + 1, k)->LightValue + 
							pStorage->GetBlock(i - 1, j + 1, k - 1)->LightValue) * 0.25f, vertexPool++);

						*indexPool++ = verticesCount;
						*indexPool++ = verticesCount + 1;
						*indexPool++ = verticesCount + 2;
						*indexPool++ = verticesCount + 3;
						*indexPool++ = verticesCount + 2;
						*indexPool++ = verticesCount + 1;

						verticesCount += 4;
					}

					block = pStorage->GetBlock(i, j-1, k);
					if(block->TypeID == ID_AIR || block->TypeID == ID_WATER)
					{
						AddToVertices(i - blockLeftX,	j - blockUpX,		k+1,	0.0f, 0.0f, bufferBlock->TypeID - 1, 0.0f, -1.0f, 0.0f, 
							(	pStorage->GetBlock(i, j - 1, k)->LightValue + 
							pStorage->GetBlock(i, j - 1, k + 1)->LightValue + 
							pStorage->GetBlock(i - 1, j - 1, k)->LightValue + 
							pStorage->GetBlock(i - 1, j - 1, k + 1)->LightValue) * 0.25f, vertexPool++);

						AddToVertices(i+1 - blockLeftX,	j - blockUpX,		k+1,	1.0f, 0.0f, bufferBlock->TypeID - 1, 0.0f, -1.0f, 0.0f, 
							(	pStorage->GetBlock(i, j - 1, k)->LightValue + 
							pStorage->GetBlock(i, j - 1, k + 1)->LightValue + 
							pStorage->GetBlock(i + 1, j - 1, k)->LightValue + 
							pStorage->GetBlock(i + 1, j - 1, k + 1)->LightValue) * 0.25f, vertexPool++);

						AddToVertices(i - blockLeftX,	j - blockUpX,		k,		0.0f, 1.0f, bufferBlock->TypeID - 1, 0.0f, -1.0f, 0.0f, 
							(	pStorage->GetBlock(i, j - 1, k)->LightValue + 
							pStorage->GetBlock(i, j - 1, k - 1)->LightValue + 
							pStorage->GetBlock(i - 1, j - 1, k)->LightValue + 
							pStorage->GetBlock(i - 1, j - 1, k - 1)->LightValue) * 0.25f, vertexPool++);

						AddToVertices(i+1 - blockLeftX,	j - blockUpX,		k,		1.0f, 1.0f, bufferBlock->TypeID - 1, 0.0f, -1.0f, 0.0f, 
							(	pStorage->GetBlock(i, j - 1, k)->LightValue + 
							pStorage->GetBlock(i, j - 1, k - 1)->LightValue + 
							pStorage->GetBlock(i + 1, j - 1, k)->LightValue + 
							pStorage->GetBlock(i + 1, j - 1, k - 1)->LightValue) * 0.25f, vertexPool++);

						*indexPool++ = verticesCount;
						*indexPool++ = verticesCount + 1;
						*indexPool++ = verticesCount + 2;
						*indexPool++ = verticesCount + 3;
						*indexPool++ = verticesCount + 2;
						*indexPool++ = verticesCount + 1;

						verticesCount += 4;
					}
				}
			}
		}
	}

	vertexPool -= verticesCount;
	indexPool -= verticesCount / 4 * 6;

	thisChunk->GetLandNode()->GetDataPtr()->indicesCount = verticesCount / 2 * 3;
	thisChunk->GetLandNode()->GetDataPtr()->verticesCount = verticesCount;
	thisChunk->GetLandNode()->GetDataPtr()->vertexSize = sizeof(LandVertex);

	mutex->lock();

	thisChunk->GetLandNode()->GetDataPtr()->Clear();
	if(!thisChunk->GetLandNode()->GetDataPtr()->CreateBuffers(false, vertexPool, thisChunk->GetLandNode()->GetDataPtr()->verticesCount * sizeof(LandVertex),
		indexPool, thisChunk->GetLandNode()->GetDataPtr()->indicesCount * sizeof(long), ShiftEngine::GetContextManager()->GetDevicePointer()))
	{
		mutex->unlock();
		return false;
	}

	mutex->unlock();

//	BuildGrassMesh(WorldX, WorldY);

	return true;
}

bool cTesselator::BuildWaterMesh( int WorldX, int WorldY )
{
	int blockLeftX = WorldX * pStorage->GetChunkWidth();
	int blockUpX = WorldY * pStorage->GetChunkWidth();
	WorldChunk * thisChunk = pChunks->GetChunkPtr(WorldX, WorldY);
	static int ChunkHeight = pStorage->GetChunkHeight();
	static int ChunkWidth = pStorage->GetChunkWidth();

	//tesselate water here
	std::vector<WaterVertex> water;
	std::vector<long> waterInd;
	int waterI = 0;
	for (int i = blockLeftX; i < blockLeftX + ChunkWidth; i++)
	{
		for (int j = blockUpX; j < blockUpX + ChunkWidth; j++)
		{
			for (int k = 0; k < ChunkHeight ; k++)
			{
				if(pStorage->GetBlock(i, j, k)->TypeID == ID_WATER && pStorage->GetBlock(i, j, k + 1)->TypeID != ID_WATER)
				{
					//add vertices for water
					float x = (float)(i - blockLeftX);
					float y = (float)(j - blockUpX);
					float z = (float)k;
					water.push_back(WaterVertex(Vector3F(x,			y,			z + 0.8f), Vector3F(0.0f, 0.0f, 1.0f)));
					water.push_back(WaterVertex(Vector3F(x + 1.0f,	y,			z + 0.8f), Vector3F(0.0f, 0.0f, 1.0f)));
					water.push_back(WaterVertex(Vector3F(x,			y + 1.0f,	z + 0.8f), Vector3F(0.0f, 0.0f, 1.0f)));
					water.push_back(WaterVertex(Vector3F(x + 1.0f,	y + 1.0f,	z + 0.8f), Vector3F(0.0f, 0.0f, 1.0f)));

					waterInd.push_back(waterI + 0);
					waterInd.push_back(waterI + 2);
					waterInd.push_back(waterI + 3);

					waterInd.push_back(waterI + 0);
					waterInd.push_back(waterI + 3);
					waterInd.push_back(waterI + 1);

					waterI += 4;
				}
			}
		}
	}

	thisChunk->GetWaterNode()->GetDataPtr()->verticesCount = water.size();
	thisChunk->GetWaterNode()->GetDataPtr()->indicesCount = waterInd.size();
	thisChunk->GetWaterNode()->GetDataPtr()->vertexSize = sizeof(WaterVertex);

	if(water.size() == 0)
		thisChunk->GetWaterNode()->SetVisibility(false);

	mutex->lock();

	thisChunk->GetWaterNode()->GetDataPtr()->Clear();

	if(!thisChunk->GetWaterNode()->GetDataPtr()->CreateBuffers(false, water.data(), water.size() * sizeof(WaterVertex),
		waterInd.data(), waterInd.size() * sizeof(long), ShiftEngine::GetContextManager()->GetDevicePointer()))
	{
		mutex->unlock();
		return false;
	}

	mutex->unlock();

	return true;
}

bool cTesselator::BuildGrassMesh( int WorldX, int WorldY )
{
	//int blockLeftX = WorldX * pStorage->GetChunkWidth();
	//int blockUpX = WorldY * pStorage->GetChunkWidth();
	//WorldChunk * thisChunk = pChunks->GetChunkPtr(WorldX, WorldY);
	//static int ChunkHeight = pStorage->GetChunkHeight();
	//static int ChunkWidth = pStorage->GetChunkWidth();

	//std::vector<texPos> grassVerts;
	//std::vector<long> grassInds;
	//int grassIndex = 0;
	//for (int i = blockLeftX; i < blockLeftX + ChunkWidth; i++)
	//{
	//	for (int j = blockUpX; j < blockUpX + ChunkWidth; j++)
	//	{
	//		generator->GetNoise()->SetOctaves(2);
	//		generator->GetNoise()->SetFrequency(0.9);
	//		double result = generator->GetNoise()->SimplexNoise((double)i / 10.0 - 16.0, (double)j / 10.0 + 11.0);

	//		generator->GetNoise()->SetOctaves(1);
	//		generator->GetNoise()->SetFrequency(1.9);
	//		double sx = generator->GetNoise()->SimplexNoise((double)i - 8.0, (double)j + 15.0) / 7.0f;
	//		double sy = generator->GetNoise()->SimplexNoise((double)i + 8.0, (double)j - 15.0) / 7.0f;

	//		if(result < 0.1)
	//			continue;

	//		float scale = result / 2.0;
	//		MathLib::clamp(scale, 0.4f, 1.0f);

	//		for (int k = 0; k < ChunkHeight - 1 ; k++)
	//		{
	//			if(pStorage->GetBlock(i, j, k)->TypeID == ID_GRASS && pStorage->GetBlock(i, j, k + 1)->TypeID == ID_AIR)
	//			{
	//				float x = (float)i + 0.5f;
	//				float y = (float)j + 0.5f;
	//				float z = (float)k + 1.0f;
	//				int addInd = AddGrass(2, grassVerts, x, y, z, 0.0f, scale * 3.0f, sx, sy, grassInds, grassIndex);
	//				grassIndex += addInd;
	//			}
	//		}
	//	}
	//}

	//thisChunk->getGrassLayerPtr()->verticesCount = grassVerts.size();
	//thisChunk->getGrassLayerPtr()->indicesCount = grassInds.size();
	//thisChunk->getGrassLayerPtr()->vertexSize = sizeof(texPos);

	//if(grassVerts.size() > 0)
	//{
	//	critSect->enter();
	//	thisChunk->getGrassLayerPtr()->Clear();
	//	thisChunk->getGrassLayerPtr()->CreateBuffers(false, grassVerts.data(), grassVerts.size() * sizeof(texPos),
	//		grassInds.data(), grassInds.size() * sizeof(long), cGraphicsEngine::GetInstance().GetDevicePointer());
	//	critSect->leave();
	//}

	return true;
}

void cTesselator::LoadGrassMeshes( const std::vector<std::wstring> & filenames )
{
	//for (auto iter = filenames.begin(); iter != filenames.end(); iter++)
	//{
	//	unsigned int vc;
	//	unsigned int ic;
	//	DefaultVertex * v;
	//	texPos * vertices;
	//	long * indices;
	//	std::pair<std::vector<texPos>, std::vector<long>> buffer;

	//	ShiftEngine::utils::getVerticesFromFile(*iter, &v, &vc, &indices, &ic);

	//	vertices = new texPos[vc];

	//	//converting vertices

	//	for (int i = 0; i < vc ; i++)
	//	{
	//		vertices[i].pos.x = v[i].Pos[0] * -1.0f;
	//		vertices[i].pos.y = v[i].Pos[2];
	//		vertices[i].pos.z = v[i].Pos[1];

	//		vertices[i].tc.x = v[i].Texcoord[0];
	//		vertices[i].tc.y = v[i].Texcoord[1];
	//	}

	//	grassMeshes.push_back(meshInfo(vertices, indices, vc, ic));

	//	delete [] v;
	//}
}

void cTesselator::SetCriticalSection( std::mutex * _mutex )
{
	mutex = _mutex;
}

int cTesselator::AddGrass( int index, std::vector<texPos> & vertDest, 
						   float x, float y, float z, 
						   float rot, float scale, float xs, float ys, 
						   std::vector<long> & indDest, long startIndex )
{
	//int vertCount = grassMeshes[index].vc;
	//int indCount = grassMeshes[index].ic;
	//texPos * vert = grassMeshes[index].vertices;
	//long * ind = grassMeshes[index].indices;

	//for (int i = 0; i < vertCount; i++)
	//{
	//	D3DXVECTOR3 p;
	//	D3DXVECTOR2 t;
	//	float av = grassMeshes[index].vertices[i].pos.z;

	//	p.x = vert[i].pos.x * scale + x + xs;
	//	p.y = vert[i].pos.y * scale + y + ys;
	//	p.z = vert[i].pos.z * scale + z;

	//	t.x = vert[i].tc.x;
	//	t.y = vert[i].tc.y;

	//	vertDest.push_back(texPos(p, t, av));
	//}

	//for (int i = 0; i < indCount; i++)
	//{
	//	indDest.push_back(ind[i] + startIndex);
	//}
	//
	//return vertCount;
	return 0;
}