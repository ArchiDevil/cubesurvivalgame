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
#include "ProducerGameObject.h"
#include "CrafterGameObject.h"
#include "PlayerGameObject.h"
#include "EntityBreeds.h"

#include "../events.h"

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
	void					Update(double dt, const Vector3F & sunPos);
	void					HighlightEntity(const Ray &unprojectedVector);
	GameObjectPtr			GetNearestEntity(const Ray &unprojectedVector);
	bool					DispatchEvent(IGameEvent * ev);

private:
	std::list<GameObjectPtr> GameObjects;
	std::unordered_map<std::string, BreedPtr> Breeds;

	GameObject * selectedEntity;
	ShiftEngine::MaterialPtr entityMaterial;

};
