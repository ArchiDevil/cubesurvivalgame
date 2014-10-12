#pragma once

#include <cstdint>

//do not shuffle block types -> colors will be wrong for types
enum BlockTypes : unsigned char
{
	BT_Empty,
	BT_Sand,
	BT_Dirt,
	BT_Grass,
	BT_Stone,
	BT_Water
};

struct BlockColumn
{
	BlockColumn() 
		: type(BT_Empty)
		, ColumnHeight(0) 
	{
	}

	BlockColumn(BlockTypes _type, uint8_t _height) 
		: type(_type)
		, ColumnHeight(_height) 
	{
	}

	BlockTypes type;
	uint8_t ColumnHeight;
};
