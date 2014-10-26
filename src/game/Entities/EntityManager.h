#pragma once

#include <list>
#include <memory>
#include <vector>

#include <SimplePhysicsEngine\PhysicsEngine.h>
#include <json/json.h>

#include "GameObject.h"
#include "PhysicsGameObject.h"
#include "ItemEntity.h"
#include "ProducerGameObject.h"
#include "CrafterGameObject.h"
#include "../Entities/PlayerGameObject.h"

#include "../events.h"

class cSimplePhysicsEngine;

using MathLib::Vector3F;

typedef std::shared_ptr<GameObject>				GameObjectPtr;
typedef std::shared_ptr<PhysicsGameObject>		PhysicsEntityPtr;
typedef std::shared_ptr<ItemEntity>				ItemEntityPtr;
typedef std::shared_ptr<CrafterGameObject>		CrafterPtr;
typedef std::shared_ptr<ProducerGameObject>		ProducerPtr;
typedef std::shared_ptr<PlayerGameObject>		PlayerPtr;

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

	ItemEntityPtr			CreateItemEntity(const Vector3F & Position, const Vector3F & Velocity, uint64_t itemId);
	GameObjectPtr			CreateEntity(const Vector3F & position, const std::string & entityId);
	CrafterPtr				CreateCrafterEntity(const Vector3F & Position, const std::string & id);
	ProducerPtr				CreateProducerEntity(const Vector3F & Position, const std::string & id);
	PlayerPtr				CreatePlayer(const Vector3F & Position);
	void					Update(double dt, const Vector3F & sunPos);
	void					SelectEntity(const MathLib::Ray &unprojectedVector);
	bool					DispatchEvent(IGameEvent * ev);

private:
	void					LoadEntities();

	std::list<GameObjectPtr> GameObjects;
	GameObject * selectedEntity;
	ShiftEngine::MaterialPtr entityMaterial;
	std::map<std::string, CrafterRepr> crafters;
	std::map<std::string, ProducerRepr> producers;

};
