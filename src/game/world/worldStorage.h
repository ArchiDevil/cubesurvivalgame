#pragma once

#include "BlockColumn.h"

#include <cassert>

class WorldStorage
{
public:
	WorldStorage(unsigned int arrayWidth) 
		: bcData(nullptr)
		, arrayWidth(arrayWidth)
	{
		assert(arrayWidth);
		bcData = new BlockColumn[arrayWidth * arrayWidth * ColumnsCount];
	}

	~WorldStorage()
	{
		delete [] bcData;
	}

	inline BlockTypes GetBlockType(int x, int y, unsigned int z) const
	{
		auto index = GetIndex(x, y, z);
		if(index != -1)
			return bcData[index].type;
		else
			return BT_Empty;
	}

	inline int GetIndex(int x, int y, unsigned int z) const
	{
		unsigned int horIndex = GetColumnIndex(x, y);
		unsigned int aggregatedHeight = 0;

		for (unsigned int i = 0; i < ColumnsCount; i++)
		{
			aggregatedHeight += bcData[horIndex + i].ColumnHeight;
			if(z < aggregatedHeight)
				return horIndex + i;
		}

		return -1;
	}

	inline int GetFullHeight(int x, int y) const
	{
		unsigned int horIndex = GetColumnIndex(x, y);
		unsigned int height = 0;
		for (unsigned int i = 0; i < ColumnsCount; i++)
			height += bcData[horIndex + i].ColumnHeight;
		return height;
	}

	inline unsigned int GetColumnIndex(int x, int y) const
	{
		int wx = x % (int)arrayWidth;
		if(wx < 0)
			wx += arrayWidth;

		int wy = y % (int)arrayWidth;
		if(wy < 0)
			wy += arrayWidth;

		return (unsigned int)wx * arrayWidth * ColumnsCount + (unsigned int)wy * ColumnsCount;
	}

	inline void SetColumn(int x, int y, unsigned int colIndex, const BlockColumn & column)
	{
		assert(colIndex < ColumnsCount);
		unsigned int horIndex = GetColumnIndex(x, y);
		bcData[horIndex + colIndex] = column;
	}

	inline unsigned int GetWorldWidth() const
	{
		return arrayWidth;
	}

	inline unsigned int GetChunkWidth() const
	{
		return ChunkWidth;
	}

	unsigned int GetColumnsCount() const
	{
		return ColumnsCount;
	}

#if defined (DEBUG) || (_DEBUG)
	inline unsigned int GetWorldSize() const
	{
		return sizeof(BlockColumn) * arrayWidth * arrayWidth * ColumnsCount;
	}
#endif

private:
	WorldStorage(const WorldStorage & ref);
	WorldStorage& operator=(const WorldStorage & ref);
	WorldStorage(WorldStorage && ref);
	WorldStorage& operator=(WorldStorage && ref);

	BlockColumn * bcData;
	unsigned int arrayWidth;
	static const unsigned int ColumnsCount = 4;
	static const unsigned int ChunkWidth = 32;

};
