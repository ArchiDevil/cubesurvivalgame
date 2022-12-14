#include "CraftingManager.h"

#include "game.h"
#include "Entities/PlayerGameObject.h"
#include "Entities/GameObjectInventory.h"
#include "Items/ItemManager.h"

#include <Utilities/ut.h>
#include <Utilities/logger.hpp>
#include <Utilities/fnv.h>

#include <json/json.h>

CraftingManager::CraftingManager()
{
}

CraftingManager::~CraftingManager()
{
}

void CraftingManager::Initialize(const std::string & path)
{
    LoadRecipes(path);
}

const Recipe& CraftingManager::GetRecipeByName(const std::string& name) const
{
    return HashRecipe.at(NameHash.at(name));
}

const Recipe& CraftingManager::GetRecipeById(recipe_id_t id) const
{
    return HashRecipe.at(id);
}

std::vector<std::pair<std::string, Recipe>> CraftingManager::GetAllRecipes() const
{
    std::vector<std::pair<std::string, Recipe>> out;
    for (const auto &key : NameHash)
        out.push_back(std::make_pair(key.first, HashRecipe.at(key.second)));

    return std::move(out);
}

void CraftingManager::LoadRecipes(const std::string & path)
{
    auto files = utils::filesystem::CollectFileNames(path, "json");
    auto pGame = LostIsland::GetGamePtr();
    auto pItemMgr = pGame->itemMgr.get();

    for (auto &file : files)
    {
        Recipe r = {};

        std::ifstream stream(path + file);
        Json::Reader reader;
        Json::Value root;
        bool result = reader.parse(stream, root);
        if (!result)
        {
            LOG_ERROR("Unable to parse entity: ", file);
            LOG_ERROR(reader.getFormattedErrorMessages());
            continue;
        }

        Json::Value null;
        std::string name = root.get("name", null).asString();
        if (name.empty())
        {
            LOG_ERROR("Unable to find name in: ", file);
            continue;
        }

        std::string producedItemName = root.get("product", null).asString();
        if (producedItemName.empty())
        {
            LOG_ERROR("Unable to find product in: ", file);
            continue;
        }

        item_id_t productId = pItemMgr->GetItemId(producedItemName);
        if (!productId)
        {
            LOG_ERROR("Unknown product: ", producedItemName, " in file: ", file);
            continue;
        }
        r.producedItem = productId;

        unsigned int count = root.get("count", null).asUInt();
        r.producedCount = count ? count : 1;

        Json::Value itemsRoot = root.get("items", null);
        if (itemsRoot == null)
        {
            LOG_ERROR("Wrong file list in: ", file);
            continue;
        }

        bool error = false;
        int i = 0;
        for (auto &item : itemsRoot)
        {
            if (error)
                break;

            if (i > 3)
            {
                LOG_ERROR("Only 4 items are supported for now. Error in: ", file);
                error = true;
                break;
            }

            std::pair<item_id_t, uint32_t> itemDesc;

            std::string itemName = item.get("name", null).asString();
            unsigned int itemCount = item.get("count", null).asUInt();

            if (itemName.empty() || !itemCount)
            {
                LOG_ERROR("Error in ingredient definition in: ", file);
                error = true;
                break;
            }

            item_id_t itemId = pItemMgr->GetItemId(itemName);
            if (!itemId)
            {
                LOG_ERROR("Unknown item: ", itemName, " in file: ", file);
                error = true;
                break;
            }

            itemDesc.first = itemId;
            itemDesc.second = itemCount;

            r.itemsToCraft[i] = itemDesc;
            i++;
        }

        if (error)
            continue;

        hash::fnv<64> hasher;
        uint64_t hash = hasher(name);

        auto hash_it = HashRecipe.find(hash);
        if (hash_it != HashRecipe.end())
        {
            LOG_ERROR("Hash collision at: ", file);
            hash++;
        }

        HashRecipe[hash] = r;

        auto name_it = NameHash.find(name);
        if (name_it != NameHash.end())
        {
            LOG_ERROR("Duplicated item: ", name, "at: ", file, ". Skipped");
            continue;
        }
        NameHash[name] = hash;
    }
}

bool CraftingManager::Craft(const Recipe& recipe)
{
    auto * pInventory = LostIsland::GetGamePtr()->player->GetInventoryPtr();
    for (auto item : recipe.itemsToCraft)
    {
        if (!pInventory->IsExist(item.first))
            return false;

        SlotUnit & slot = pInventory->FindSlotWithItem(item.first);
        if (slot.count < item.second)
            return false; // not enough items to craft it
    }

    // after all checks we need to remove items from inventory and add new crafted item to the player
    for (auto item : recipe.itemsToCraft)
    {
        SlotUnit & slot = pInventory->FindSlotWithItem(item.first);
        slot.count -= item.second;
        if (slot.count == 0)
            slot.itemId = 0;
    }

    pInventory->AddItem(recipe.producedItem, recipe.producedCount);
    return true;
}
