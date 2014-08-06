#pragma once

#include <list>
#include <memory>
#include <vector>

#include "GameObject.h"
#include "PhysicsEntity.h"
#include "ItemEntity.h"
#include "..\SimplePhysicsEngine\PhysicsEngine.h"

class cSimplePhysicsEngine;

typedef std::shared_ptr<GameObject>		GameObjectPtr;
typedef std::shared_ptr<PhysicsEntity>	PhysicsEntityPtr;
typedef std::shared_ptr<ItemEntity>		ItemEntityPtr;

class cEntityManager
{
public:
	cEntityManager();
	~cEntityManager();

	void					 Initialize();
	ItemEntityPtr			 CreateItemEntity(Vector3D Position, Vector3D Velocity, cItem * item);
	GameObjectPtr			 FindEntityByPos(Vector3F Position);
	vector<ItemEntity *>	 FindItemsNearPlayer(Vector3F playerPos);
	void					 Update(double dt, const Vector3F & sunPos);
	void					 DrawAll(Vector3F lightPos);

private:
	std::list<GameObjectPtr> GameObjects;
	ShiftEngine::MaterialPtr entityMaterial;

};