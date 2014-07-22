#pragma once

#include "block.h"

class cWorldStorage
{
	friend class cWorld;
	friend class cChunkStreamer;

public:
	cWorldStorage() 
		: WorldData(nullptr), ArrayWidth(0), Size(0)
	{
	}

	~cWorldStorage()
	{
		delete [] WorldData;
	}

	__forceinline unsigned int GetChunkWidth()
	{
		return ChunkWidth;
	}

	__forceinline unsigned int GetChunkHeight()
	{
		return ChunkHeight;
	}

	__forceinline Block * GetBlock(int x, int y, int z)
	{
		return &WorldData[GetIndex(x, y, z)];
	}

	__forceinline Block * GetBlock(unsigned int PTR)
	{
		return &WorldData[PTR];
	}

	__forceinline int GetIndex(int x, int y, int z)
	{
		int wx = x;
		if(x >= ArrayWidth)
			wx %= ArrayWidth;
		if(wx < 0)
			wx += ArrayWidth;

		int wy = y;
		if(y >= ArrayWidth)
			wy %= ArrayWidth;
		if(wy < 0)
			wy += ArrayWidth;

		unsigned int index = ChunkHeight * (wx * ArrayWidth + wy) + z;

		return index;
	}

private:
	int ArrayWidth;
	int Size;

	Block * WorldData;

	static const unsigned int ChunkWidth = 32;
	static const unsigned int ChunkHeight = 256;
	
};