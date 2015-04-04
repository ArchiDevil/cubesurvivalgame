#include "ItemManager.h"

#include "Item.h"
#include "MiscItem.h"
#include "WeaponItem.h"
#include "FoodItem.h"
#include "EntityItem.h"

#include <Utilities/fnv.h>
#include <json/json.h>
#include <GraphicsEngine/ShiftEngine.h>
#include <Utilities/logger.hpp>
#include <Utilities/ut.h>

#include <cassert>

void ItemManager::Initialize(const std::string & PathName)
{
    LoadDefinitions(PathName);
}

void ItemManager::LoadDefinitions(const std::string & path)
{
    auto files = utils::filesystem::CollectFileNames(path);
    for (auto & file : files)
    {
        if (utils::filesystem::IsDirectory(path + file))
        {
            LoadDefinitions(path + file + "/");
            continue;
        }

        //load this file, yeah
        std::ifstream stream(path + file);
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
                LOG_ERROR("Unable to get required key ", key, " from ", file);
            }
        }

        if (errorFlag) continue;

        std::string id, name, type, description, iconName;
        id = root.get("id", null).asString();
        name = root.get("name", null).asString();
        type = root.get("type", null).asString();
        iconName = root.get("icon", null).asString();
        description = root.get("description", null).asString();

        hash::fnv<64> hasher;
        uint64_t hash = hasher(id);
        auto itemIter = HashItem.find(hash);
        if (itemIter != HashItem.end())
        {
            LOG_ERROR("Hash collision on: ", id);
            ++hash;
        }

        auto nameIter = NameHash.find(id);
        if (nameIter != NameHash.end())
        {
            LOG_ERROR("Name collision of: ", id, " and ", nameIter->first);
        }
        NameHash[id] = hash;

        auto cubeMesh = ShiftEngine::Utilities::createCube();
        auto iconTexture = ShiftEngine::GetContextManager()->LoadTexture(utils::Widen(iconName));

        if (type == "misc")
        {
            HashItem[hash] = new MiscItem(name, description, cubeMesh, iconTexture, iconName);
        }
        else if (type == "weapon")
        {
            int damageCount = root.get("damage", null).asInt();
            HashItem[hash] = new WeaponItem(name, description, cubeMesh, iconTexture, damageCount, iconName);
        }
        else if (type == "food")
        {
            int hungerCount = root.get("hunger", null).asInt();
            HashItem[hash] = new FoodItem(name, description, cubeMesh, iconTexture, hungerCount, iconName);
        }
        else if (type == "entity")
        {
            std::string entityId = root.get("entity", null).asString();
            HashItem[hash] = new EntityItem(name, description, cubeMesh, iconTexture, entityId, iconName);
        }
        else
        {
            LOG_ERROR("Unable to resolve item type: ", type);
        }
    }
}

Item * ItemManager::GetItemById(item_id_t itemId)
{
    auto iter = HashItem.find(itemId);
    if (iter != HashItem.end())
        return iter->second;

    return nullptr;
}

item_id_t ItemManager::GetItemId(const std::string & ItemName)
{
    auto iter = NameHash.find(ItemName);
    if (iter != NameHash.end())
        return iter->second;

    return 0ULL;
}
