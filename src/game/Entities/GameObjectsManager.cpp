#include "GameObjectsManager.h"

#include <vector>
#include <algorithm>

#include <SimplePhysicsEngine/PhysicsEngine.h>

#include <GraphicsEngine/ShiftEngine.h>

#include <Utilities/ut.h>

#include <json/json.h>

#include "../game.h"

using namespace ShiftEngine;

GameObjectsManager::GameObjectsManager()
{
    IProgramPtr EntityShader = GetContextManager()->LoadShader(L"EntityShader.fx");
    entityMaterial = GetContextManager()->LoadMaterial(L"entity.mtl", L"genericEntity");
}

ItemGameObjectPtr GameObjectsManager::CreateItemEntity(const MathLib::Vector3F & Position, const MathLib::Vector3F & Velocity, uint64_t itemId, size_t count)
{
    auto pGame = LostIsland::GetGamePtr();
    auto * item = pGame->itemMgr->GetItemById(itemId);
    if (!item)
    {
        LOG_ERROR("Unable to create item entity with ", itemId, " itemId");
        return nullptr;
    }

    float scale = 0.15f;
    MathLib::AABB bbox = { MathLib::Vector3F(-0.5f, -0.5f, -0.5f), MathLib::Vector3F(0.5f, 0.5f, 0.5f) };
    bbox.bMin *= scale;
    bbox.bMax *= scale;
    MeshNode * meshNode = GetSceneGraph()->AddMeshNode(item->GetMesh(), entityMaterial.get());
    std::shared_ptr<ItemGameObject> out = std::make_shared<ItemGameObject>(itemId, count, SimplePhysicsEngine::GetInstance().CreateEntity(Position, Velocity, bbox), meshNode);
    meshNode->GetMaterialPtr()->SetDiffuseTexture(item->GetTexturePtr());
    meshNode->SetScale(scale);
    gameObjects.push_back(out);
    return out;
}

void GameObjectsManager::Update(double dt)
{
    auto iter = gameObjects.begin();
    while (iter != gameObjects.end())
    {
        (*iter)->Update(dt);
        if ((*iter)->MustBeDeleted())
            iter = gameObjects.erase(iter);
        else
            ++iter;
    }
}

GameObjectPtr GameObjectsManager::CreateEntity(const MathLib::Vector3F & position, const std::string & entityId)
{
    auto breedIter = breeds.find(entityId);
    if (breedIter == breeds.end())
    {
        LOG_ERROR("Unable to create entity with id: ", entityId);
        return nullptr;
    }

    auto out = breedIter->second->Clone();
    out->SetPosition(position);
    gameObjects.push_back(out);
    return out;
}

PlayerPtr GameObjectsManager::CreatePlayer(const MathLib::Vector3F & Position)
{
    auto pGame = LostIsland::GetGamePtr();
    if (pGame->player)
    {
        LOG_ERROR("Player is already created");
        return nullptr;
    }
    auto pCtxMgr = ShiftEngine::GetContextManager();
    auto pScene = ShiftEngine::GetSceneGraph();

    ShiftEngine::MaterialPtr mat = pCtxMgr->LoadMaterial(L"player.mtl", L"player");
    PlayerPtr player = std::make_shared<PlayerGameObject>(pScene->AddMeshNode(ShiftEngine::Utilities::createCube(), mat.get()), pGame->itemMgr.get());
    player->SetPosition(Position);
    gameObjects.push_back(player);
    pGame->player = player.get();
    return player;
}

void GameObjectsManager::HighlightEntity(const MathLib::Ray &unprojectedRay)
{
    if (selectedEntity && selectedEntity->CanBeHighlighted(unprojectedRay))
        return;

    bool selected = false;
    for (auto & entity : gameObjects)
    {
        if (entity->CanBeHighlighted(unprojectedRay))
        {
            entity->Highlight();
            if (selectedEntity)
                selectedEntity->UnHightlight();
            selectedEntity = entity.get();
            selected = true;
            break;
        }
    }

    if (!selected)
    {
        if (selectedEntity)
            selectedEntity->UnHightlight();
        selectedEntity = nullptr;
    }
}

GameObjectPtr GameObjectsManager::GetNearestEntity(const MathLib::Ray &unprojectedRay)
{
    for (auto & entity : gameObjects)
        if (entity->CanBeHighlighted(unprojectedRay))
            return entity;
    return nullptr;
}

void GameObjectsManager::LoadEntities()
{
    LoadInventories();

    auto *pGame = LostIsland::GetGamePtr();

    std::wstring pathPrefix = L"resources/gamedata/entities/";
    auto files = utils::filesystem::CollectFileNames(pathPrefix);

    Json::Reader reader;
    for (auto & file : files)
    {
        file = pathPrefix + file;
        std::string filename = utils::Narrow(file);
        std::ifstream jsonfile(filename);
        if (jsonfile.fail() || jsonfile.bad())
        {
            LOG_ERROR("Unable to open ", filename);
            continue;
        }

        Json::Value root(0);
        bool result = reader.parse(jsonfile, root);
        if (!result)
        {
            LOG_ERROR("Unable to parse ", filename);
            LOG_ERROR(reader.getFormatedErrorMessages());
            continue;
        }

        //check common values
        std::string props[] = {
            "id", "mesh", "material", "type"
        };

        Json::Value buff(0);
        bool errorFlag = false;
        for (auto & prop : props)
        {
            buff = root.get(prop, buff);
            if (buff.empty())
            {
                LOG_ERROR("Wrong entity ", filename);
                LOG_ERROR("Unable to find property ", prop);
                errorFlag = true;
            }
        }
        if (errorFlag) continue;

        std::string id = root.get("id", buff).asString();
        std::string meshName = root.get("mesh", buff).asString();
        std::string materialName = root.get("material", buff).asString();
        buff = { 1.0f };
        buff = root.get("scale", buff);
        float scale = 1.0f;
        scale = buff.asFloat();

        buff = root.get("type", buff);
        std::string type = buff.asString();
        if (type == "static")
        {
            std::string inventoryName = root.get("inventory", buff).asString();
            InventoryBreed emptyBreed;
            if (!inventoryName.empty())
                breeds[id] = std::make_shared<LiveBreed>(inventories[inventoryName], meshName, materialName, scale);
            else
                breeds[id] = std::make_shared<LiveBreed>(emptyBreed, meshName, materialName, scale);
        }
        else if (type == "collectable")
        {
            buff = root.get("item", buff);
            std::string itemName = buff.asString();
            if (itemName.empty())
            {
                LOG_ERROR("Unable to find entity item for: ", id);
                continue;
            }
            item_id_t itemId = pGame->itemMgr->GetItemId(itemName);
            buff = root.get("count", buff);
            unsigned count = buff.asUInt();
            breeds[id] = std::make_shared<CollectableBreed>(meshName, materialName, itemId, count, scale);
        }
        else if (type == "heater")
        {
            buff = root.get("heat_count", buff);
            int heatCount = buff.asInt();
            breeds[id] = std::make_shared<HeaterBreed>(meshName, materialName, heatCount, scale);
        }
        else
        {
            LOG_ERROR("Unable to parse ", filename, ": unknown entity type");
            continue;
        }
    }
}

void GameObjectsManager::LoadInventories()
{
    std::wstring pathPrefix = L"resources/gamedata/inventories/";
    auto files = utils::filesystem::CollectFileNames(pathPrefix);

    auto *pGame = LostIsland::GetGamePtr();

    Json::Reader reader;
    for (const auto & file_name : files)
    {
        std::wstring file = pathPrefix + file_name;
        std::string filename = utils::Narrow(file);
        std::ifstream jsonfile(filename);
        if (jsonfile.fail() || jsonfile.bad())
        {
            LOG_ERROR("Unable to open ", filename);
            continue;
        }

        Json::Value root(0);
        bool result = reader.parse(jsonfile, root);
        if (!result)
        {
            LOG_ERROR("Unable to parse ", filename);
            LOG_ERROR(reader.getFormatedErrorMessages());
            continue;
        }

        Json::Value buff(0);
        buff = root.get("items", buff);
        if (buff.empty() || !buff.isArray())
        {
            LOG_ERROR("Unable to find 'items' array-node in ", filename, ". Skipped.");
            continue;
        }

        auto itemsCount = buff.size();
        InventoryBreed inventory;
        for (unsigned int i = 0; i < itemsCount; ++i)
        {
            Json::Value null(0);
            buff = buff.get(i, null);
            if (buff.empty())
            {
                LOG_ERROR("Wrong array element #", i, " in file ", filename);
                continue;
            }

            std::string itemName = buff.get("item", null).asString();
            if (!itemName.size())
            {
                LOG_ERROR("Wrong item name in element #", i, " in file ", filename);
                continue;
            }

            unsigned int count = buff.get("count", null).asUInt();
            unsigned int min_count = buff.get("min_count", null).asUInt();
            unsigned int max_count = buff.get("max_count", null).asUInt();

            if (!count && !min_count && !max_count)
            {
                LOG_ERROR("Wrong item count in element #", i, " in file ", filename);
                continue;
            }

            if (count && (min_count || max_count))
            {
                LOG_ERROR("Wrong combination of count/min_count/max_count in element #", i, " in file ", filename);
                continue;
            }

            if (count)
                inventory.AddItem(InventoryBreed::ExtendedSlotUnit(pGame->itemMgr->GetItemId(itemName), count));

            if (min_count || max_count)
                inventory.AddItem(InventoryBreed::ExtendedSlotUnit(pGame->itemMgr->GetItemId(itemName), 0, min_count, max_count));
        }

        inventories.emplace(std::make_pair(utils::Narrow(file_name.substr(0, file_name.find(L"."))), std::move(inventory)));
    }
}

void GameObjectsManager::DispatchEvent(std::unique_ptr<IGameEvent> ev)
{
    for (auto & el : gameObjects)
    {
        el->DispatchEvent(ev.get());
    }
}
