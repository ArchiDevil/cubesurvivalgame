#pragma once

#include "WorldChunk.h"

class cChunksStorage
{
public:
	cChunksStorage();
	~cChunksStorage();

	void Initialize(int ChunksPerSide, int CenterChunkX, int CenterChunkY, unsigned int chunkWidth);	
	void Unload();

	int GetChunksPerSide() const;
	int GetCentralX() const;
	int GetCentralY() const;

	int GetChunkNumPointer(int WorldX, int WorldY) const;
	WorldChunk * GetChunkPtr(int WorldX, int WorldY);
	const WorldChunk * GetChunkPtr(int WorldX, int WorldY) const;
	WorldChunk * GetChunksArray();
	const WorldChunk * GetChunksArray() const;

	void SetCenterX(int x);
	void SetCenterY(int y);

	bool IsBorder(int WorldX, int WorldY) const;
	bool IsExist(int WorldX, int WorldY) const;
	bool HaveRightNeighbors(int WorldX, int WorldY, ChunkStatus minimalStatus) const;

private:
	WorldChunk * Chunks;
	ShiftEngine::TexturePtr TextureArray;

	int CenterChunkX, CenterChunkY;
	int ChunksPerSide;
	
};