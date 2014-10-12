#include "ItemManager.h"

#include "Item.h"
#include "MiscItem.h"
#include "WeaponItem.h"
#include "FoodItem.h"
#include "EntityItem.h"

#include <cassert>

#include <Utilities/fnv.h>
#include <json/json.h>
#include <GraphicsEngine/ShiftEngine.h>

ItemManager::ItemManager( PlayerGameObject * _p, cWorld * _w, typesStorage * _ts )
	: ts(_ts)
	, handler(_p, _w)
{
}

ItemManager::~ItemManager() 
{
}

void ItemManager::Initialize( const std::wstring & PathName )
{
	LoadDefinitions(PathName);
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
		Json::Value root, null;
		reader.parse(stream, root);

		//check required properties
		std::string required[] = {
			"id", "type", "icon"
		};
		
		bool errorFlag = false;

		for (auto & key : required)
		{
			if (root.get(key, null).empty())
			{
				errorFlag = true;
				MainLog.Error("Unable to get required key " + key + " from " + utils::WStrToStr(file));
			}
		}

		if (errorFlag) continue;

		std::string id, type, description, iconName;
		id = root.get("id", null).asString();
		type = root.get("type", null).asString();
		iconName = root.get("icon", null).asString();
		description = root.get("description", null).asString();

		hash::fnv<64> hasher;
		uint64_t hash = hasher(id);
		auto itemIter = HashItem.find(hash);
		if (itemIter != HashItem.end())
		{
			MainLog.Error("Hash collision on: " + id);
			++hash;
		}

		auto nameIter = NameHash.find(id);
		if (nameIter != NameHash.end())
		{
			MainLog.Error("Name collision of: " + id + " and " + nameIter->first);
		}
		NameHash[id] = hash;

		auto cubeMesh = ShiftEngine::Utilities::createCube();
		auto iconTexture = ShiftEngine::GetContextManager()->LoadTexture(utils::StrToWStr(iconName));

		if (type == "misc")
		{
			HashItem[hash] = new MiscItem(&handler, id, description, cubeMesh, iconTexture);
		}
		else if (type == "weapon")
		{
			int damageCount = root.get("damage", null).asInt();
			HashItem[hash] = new WeaponItem(&handler, id, description, cubeMesh, iconTexture, damageCount);
		}
		else if (type == "food")
		{
			int hungerCount = root.get("hunger", null).asInt();
			HashItem[hash] = new FoodItem(&handler, id, description, cubeMesh, iconTexture, hungerCount);
		}
		else if (type == "entity")
		{
			std::string entityId = root.get("entity", null).asString();
			HashItem[hash] = new EntityItem(&handler, id, description, cubeMesh, iconTexture, entityId);
		}
		else
		{
			MainLog.Error("Unable to resolve item type: " + type);
		}
	}
}

uint64_t ItemManager::GetBlockItem( BlockType type )
{
	std::string blockName = std::to_string((int)type);
	auto iter = NameHash.find(blockName);
	if (iter != NameHash.end())
		return iter->second;

	return 0;
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
