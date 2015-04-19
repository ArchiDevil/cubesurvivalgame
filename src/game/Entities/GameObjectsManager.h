#pragma once

#include <list>
#include <memory>
#include <vector>
#include <unordered_map>

#include <Utilities/IManager.h>

#include "GameObject.h"
#include "PhysicsGameObject.h"
#include "ItemGameObject.h"
#include "PlayerGameObject.h"
#include "EntityBreeds.h"
#include "../GameEvents.h"

class cSimplePhysicsEngine;
using MathLib::Vector3F;
using MathLib::Ray;

class GameObjectsManager : public IManager
{
public:
    GameObjectsManager();
    void                LoadEntities();

    ItemGameObjectPtr       CreateItemEntity(const Vector3F & Position, const Vector3F & Velocity, uint64_t itemId, size_t count);
    GameObjectPtr           CreateEntity(const Vector3F & position, const std::string & entityId);
    PlayerPtr               CreatePlayer(const Vector3F & Position);
    void                    Update(double dt);
    void                    HighlightEntity(const Ray &unprojectedRay);
    GameObjectPtr           GetNearestEntity(const Ray &unprojectedRay);
    void                    DispatchEvent(std::unique_ptr<IGameEvent> ev);

private:
    void                    LoadInventories();

    std::list<GameObjectPtr>                        gameObjects;
    std::unordered_map<std::string, InventoryBreed> inventories;
    std::unordered_map<std::string, BreedPtr>       breeds;

    GameObject *                selectedEntity = nullptr;
    ShiftEngine::MaterialPtr    entityMaterial = nullptr;

};
