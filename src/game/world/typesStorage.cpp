#include "typesStorage.h"

typesStorage::typesStorage()
{}

typesStorage::~typesStorage()
{}

void typesStorage::loadTypes(std::vector<std::wstring> & vec)
{
// 	int textureIndex = 0;
// 	for (auto iter = vec.begin(); iter != vec.end(); iter++)
// 	{
// 		textureIndex += Load(*iter);
// 	}

	//for (int i = 0; i < ID_END ; i++)
	//{
	//	types[i].setTextureIndex(i - 1);
	//}

	textureNames.push_back(std::wstring(L"worldroot.jpg"));
	textureNames.push_back(std::wstring(L"stone.jpg"));
	textureNames.push_back(std::wstring(L"grass.jpg"));
	textureNames.push_back(std::wstring(L"dirt.jpg"));
	textureNames.push_back(std::wstring(L"sand.jpg"));
	textureNames.push_back(std::wstring(L"empty-green.png"));
}

int typesStorage::Load( std::wstring name )
{
	return 0;
}

std::vector<std::wstring> & typesStorage::getTexturesNames()
{
	return textureNames;
}

//blockInfo * typesStorage::getTypeInfo( BlockType type )
//{
//	return &types[type];
//}
