#pragma once

#include <list>
#include <memory>
#include <vector>
#include <unordered_map>

#include <SimplePhysicsEngine\PhysicsEngine.h>
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

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	ItemGameObjectPtr		CreateItemEntity(const Vector3F & Position, const Vector3F & Velocity, uint64_t itemId);
	GameObjectPtr			CreateEntity(const Vector3F & position, const std::string & entityId);
	PlayerPtr				CreatePlayer(const Vector3F & Position);
	void					Update(double dt, const Vector3F & sunPos);
	void					SelectEntity(const MathLib::Ray &unprojectedVector);
	GameObjectPtr			GetNearestEntity(const MathLib::Ray &unprojectedVector);
	bool					DispatchEvent(IGameEvent * ev);

private:
	void					LoadEntities();

	std::list<GameObjectPtr> GameObjects;
	std::unordered_map<std::string, BreedPtr> Breeds;

	GameObject * selectedEntity;
	ShiftEngine::MaterialPtr entityMaterial;

};
