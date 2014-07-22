#pragma once

#include "datatypes.h"

#pragma pack(push, 1)

struct Block
{
	Block() 
		: TypeID((BlockType)0), LightValue(0){};
	BlockType TypeID;				//��� �����
	unsigned char LightValue :4;	//�������� ������������
};

#pragma pack(pop)