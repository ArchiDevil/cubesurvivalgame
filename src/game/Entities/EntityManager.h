#pragma once

#include <list>
#include <memory>
#include <vector>
#include <unordered_map>

#include <Utilities/IManager.h>
#include <SimplePhysicsEngine/PhysicsEngine.h>
#include <json/json.h>

#include "GameObject.h"
#include "PhysicsGameObject.h"
#include "ItemGameObject.h"
#include "CrafterGameObject.h"
#include "PlayerGameObject.h"
#include "EntityBreeds.h"

class cSimplePhysicsEngine;
using MathLib::Vector3F;
using MathLib::Ray;

class EntityManager : public IManager
{
public:
	EntityManager();
	~EntityManager();
	void					LoadEntities();

	ItemGameObjectPtr		CreateItemEntity(const Vector3F & Position, const Vector3F & Velocity, uint64_t itemId);
	GameObjectPtr			CreateEntity(const Vector3F & position, const std::string & entityId);
	PlayerPtr				CreatePlayer(const Vector3F & Position);
	void					Update(double dt);
	void					HighlightEntity(const Ray &unprojectedVector);
	GameObjectPtr			GetNearestEntity(const Ray &unprojectedVector);

private:
	std::list<GameObjectPtr> GameObjects;
	std::unordered_map<std::string, BreedPtr> Breeds;

	GameObject * selectedEntity;
	ShiftEngine::MaterialPtr entityMaterial;

};
