#pragma once

#include <mutex>

#include "WorldChunk.h"
#include "cChunksStorage.h"
#include "cWorldStorage.h"
#include "worldGenerator/cWorldGenerator.h"

class cTesselator
{
public:
	void Initialize(cWorldStorage * ws, cChunksStorage * Chunks, cWorldGenerator * gen);
	bool BuildChunkMesh(int WorldX, int WorldY);
	bool BuildWaterMesh(int WorldX, int WorldY);

	void SetCriticalSection(std::mutex * section);

private:
	__forceinline void AddToVertices(int posX, int posY, int posZ, 
		float texX, float texY, int texZ,
		float normX, float normY, float normZ,
		float lightness, LandVertex * pVertices)
	{
		pVertices->position.x = (float)posX;
		pVertices->position.y = (float)posY;
		pVertices->position.z = (float)posZ;
		pVertices->texcoord.x = texX;
		pVertices->texcoord.y = texY;
		pVertices->texcoord.z = (float)texZ;
		pVertices->normal.x =	normX;
		pVertices->normal.y =	normY;
		pVertices->normal.z =	normZ;
		pVertices->color =		lightness;
	};

	int AddGrass(int index, std::vector<texPos> & vertDest, 
		float x, float y, float z, 
		float rot, float scale, float xs, float ys,
		std::vector<long> & indDest, long startIndex);

	bool BuildGrassMesh(int WorldX, int WorldY);
	void LoadGrassMeshes(const std::vector<std::wstring> & filenames);
	
	cWorldStorage *		pStorage;
	cChunksStorage *	pChunks;
	cWorldGenerator *	generator;
	std::mutex *	    mutex;

	int					maxVertices;

	struct meshInfo
	{
		meshInfo(texPos * ver, long * ind, unsigned long _vc, unsigned long _ic) 
			: vertices(ver), indices(ind), vc(_vc), ic(_ic) {};
		texPos * vertices;
		long * indices;
		unsigned long vc, ic;
	};

	std::vector< meshInfo > grassMeshes;

	// pools for very fast chunk build and avoid 
	// two passes for chunk
	LandVertex *		vertexPool;
	long *				indexPool;

};