#pragma once

#include <list>
#include <memory>
#include <vector>

#include <SimplePhysicsEngine\PhysicsEngine.h>
#include <json/json.h>

#include "GameObject.h"
#include "PhysicsEntity.h"
#include "ItemEntity.h"
#include "ProducerGameObject.h"
#include "CrafterGameObject.h"

#include "../events.h"

class cSimplePhysicsEngine;

typedef std::shared_ptr<GameObject>				GameObjectPtr;
typedef std::shared_ptr<PhysicsEntity>			PhysicsEntityPtr;
typedef std::shared_ptr<ItemEntity>				ItemEntityPtr;
typedef std::shared_ptr<CrafterGameObject>		CrafterPtr;
typedef std::shared_ptr<ProducerGameObject>		ProducerPtr;

class EntityManager
{
	struct EntityRepr
	{
		std::string meshName;
		std::string materialName;
	};

	struct CrafterRepr : public EntityRepr
	{
		Item * craftingItem;
		uint32_t craftingTime;
	};

	struct ProducerRepr : public EntityRepr
	{
		Item * producedItem;
		uint32_t producingTime;
	};

public:
	EntityManager();
	~EntityManager();

	ItemEntityPtr			CreateItemEntity(const Vector3D & Position, const Vector3D & Velocity, uint64_t itemId);
	GameObjectPtr			CreateEntity(const MathLib::Vector3F & position, const std::string & entityId);
	CrafterPtr				CreateCrafterEntity(const Vector3F & Position, const std::string & id);
	ProducerPtr				CreateProducerEntity(const Vector3F & Position, const std::string & id);
	void					Update(double dt, const Vector3F & sunPos);

	bool					DispatchEvent(IGameEvent * ev);

private:
	void					LoadEntities();

	std::list<GameObjectPtr> GameObjects;
	ShiftEngine::MaterialPtr entityMaterial;

	//std::unordered_map<std::string, std::string> entitiesDb;

	std::map<std::string, CrafterRepr> crafters;
	std::map<std::string, ProducerRepr> producers;

};
