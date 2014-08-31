#include "ItemManager.h"

#include "Item.h"
#include "BlockItem.h"
#include "MiscItem.h"
#include "WeaponItem.h"

#include <cassert>

#include <Utilities/fnv.h>
#include <json/json.h>
#include <GraphicsEngine/ShiftEngine.h>

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
	auto files = utils::filesystem::CollectFileNames(path);
	for (auto & file : files)
	{
		if (utils::filesystem::IsDirectory(path + file))
		{
			LoadDefinitions(path + file + L"/");
			continue;
		}

		//load this file, yeah
		std::ifstream stream(utils::WStrToStr(path + file));
		Json::Reader reader;
		Json::Value root;
		Json::Value buffer, null;
		reader.parse(stream, root);

		//check required properties
		std::string required[] = {
			"id", "type", "icon"
		};
		
		bool errorFlag = false;

		for (auto & key : required)
		{
			buffer = root.get(key, null);
			if (buffer.empty())
			{
				errorFlag = true;
				MainLog.Error("Unable to get required key " + key + " from " + utils::WStrToStr(file));
			}
		}

		if (errorFlag) continue;

		std::string id, type, description, icon;
		id = root.get("id", null).asString();
		type = root.get("type", null).asString();
		icon = root.get("icon", null).asString();
		description = root.get("description", null).asString();

		hash::fnv<64> hasher;
		uint64_t hash = hasher(id);
		auto itemIter = HashItem.find(hash);
		if (itemIter != HashItem.end())
		{
			MainLog.Error("Hash collision on: " + id);
			++hash;
		}
		HashItem[hash] = nullptr;

		auto nameIter = NameHash.find(id);
		if (nameIter != NameHash.end())
		{
			MainLog.Error("Name collision of: " + id + " and " + nameIter->first);
		}
		NameHash[id] = hash;

		MainLog.Message("ID: " + id + "; TYPE: " + type + "; ICON: " + icon + "; DESC: " + description + "; HASH: " + std::to_string(hash));
	}
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

Item * ItemManager::GetBlockItem( BlockType type )
{
	return Blocks[(int)type];
}

Item * ItemManager::GetItemById(uint64_t itemId)
{
	auto iter = HashItem.find(itemId);
	if (iter != HashItem.end())
		return iter->second;

	return nullptr;
}

uint64_t ItemManager::GetItemId(const std::string & ItemName)
{
	auto iter = NameHash.find(ItemName);
	if (iter != NameHash.end())
		return iter->second;

	return 0ULL;
}
