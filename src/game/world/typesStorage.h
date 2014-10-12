#pragma once

#include <vector>

//#include "blockInfo.h"

class typesStorage
{
public:
	typesStorage();
	~typesStorage();

	void loadTypes(std::vector<std::wstring> & vec);
	std::vector<std::wstring> & getTexturesNames();
	//blockInfo * getTypeInfo(BlockType type);

private:
	int Load(std::wstring name);

	std::vector<std::wstring> textureNames;
	//blockInfo types[ID_END];

	// должен хранить: 
	// типы блоков
	// адреса текстур для типов (в 3д массиве)
};
