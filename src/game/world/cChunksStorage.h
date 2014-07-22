#pragma once

#include "WorldChunk.h"
#include "typesStorage.h"

class cChunksStorage
{
public:
	cChunksStorage();
	~cChunksStorage();

	void Initialize(int ChunksPerSide, int CenterChunkX, int CenterChunkY, unsigned int chunkWidth, unsigned int chunkHeight, typesStorage * ts);		//������������� :3
	void Unload();

	int GetChunksPerSide();		//���������� ���������� ������ �� ������� ����
	int GetCenterX();			//���������� ����������� ���� �� X
	int GetCenterY();			//���������� ����������� ���� �� Y

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