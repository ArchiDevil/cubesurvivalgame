#pragma once

#include "WorldChunk.h"
#include "typesStorage.h"

class cChunksStorage
{
public:
	cChunksStorage();
	~cChunksStorage();

	void Initialize(int ChunksPerSide, int CenterChunkX, int CenterChunkY, unsigned int chunkWidth, unsigned int chunkHeight, typesStorage * ts);		//инициализация :3
	void Unload();

	int GetChunksPerSide();		//возвращает количество чанков на сторону мира
	int GetCenterX();			//возвращает центральный чанк по X
	int GetCenterY();			//возвращает центральный чанк по Y

	int GetChunkNumPointer(int WorldX, int WorldY);
	WorldChunk * GetChunkPtr(int WorldX, int WorldY);
	WorldChunk * GetChunksArray();

	void SetCenterX(int x);
	void SetCenterY(int y);

	bool IsBorder(int WorldX, int WorldY) const;
	bool IsExist(int WorldX, int WorldY) const;
	bool HaveRightNeighbors(int WorldX, int WorldY, ChunkStatus minimalStatus);

private:
	WorldChunk * Chunks;
	ShiftEngine::TexturePtr TextureArray;

	int CenterChunkX, CenterChunkY;
	int ChunksPerSide;
	
};