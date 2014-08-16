#include "ItemManager.h"

#include <cassert>

#include "../../GraphicsEngine/ShiftEngine.h"

ItemManager::ItemManager( cPlayer * _p, cWorld * _w, typesStorage * _ts )
	: handler(_p, _w), ts(_ts) {}

ItemManager::~ItemManager() {}

void ItemManager::Initialize( const std::wstring & PathName )
{
	LoadDefinitions(PathName);
	LoadBlocks();
}

void ItemManager::LoadDefinitions( const std::wstring & path )
{
	Items["empty"] = new MiscItem(nullptr, "null", "null", nullptr, nullptr);	//empty item

	std::wstring str = path;

	auto filenames = ::utils::CollectFileNames(path, L"itm");
	for(auto iter = filenames.begin(); iter != filenames.end(); iter++)
		LoadItem(str + *iter);
}

void ItemManager::LoadItem( const std::wstring & filename )
{
}

void ItemManager::LoadBlocks()
{
	static_assert(ID_END == 7U, "Update loop!");
	std::vector<std::wstring> textures = ts->getTexturesNames();

	Item * blockAir = new BlockItem(&handler, "0", "0", nullptr, ID_AIR);
	Blocks.push_back(blockAir);

	assert(ShiftEngine::GetContextManager() != 0);

	for (int index = (int)ID_WROOT; index <= (int)ID_SAND; index++)
	{
		Item * block = new BlockItem(&handler, std::to_string(index), std::to_string(index), 
			ShiftEngine::GetContextManager()->LoadTexture(textures[index - 1]), (BlockType)index);
		Blocks.push_back(block);
	}
}

Item * ItemManager::GetItemByName( const std::string & Name )
{
	return Items[Name];
}

Item * ItemManager::GetBlockItem( BlockType type )
{
	return Blocks[(int)type];
}
