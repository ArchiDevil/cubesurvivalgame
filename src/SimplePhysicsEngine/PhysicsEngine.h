#pragma once

#include <list>

#include <game/world/worldStorage.h>
#include "PhysicsFunctions.h"
#include "types.h"
#include "CollisionShouter.h"

#include <Utilities/singleton.h>

class cSimplePhysicsEngine : public singleton<cSimplePhysicsEngine>
{
public:
	cSimplePhysicsEngine();
	~cSimplePhysicsEngine();

	void Initialize(WorldStorage * storage, float gravity = -9.81f);
	void Update(double dt);

	PhysObjectPtr CreateEntity(const Vector3F & Position, const Vector3F & Velocity);
	int GetPhysEntitySize() const;
	std::vector<CollisionInfo> & GetCollisionsPlayerEntity();

private:
	void UpdatePhysics(double dt);

	CollisionListener collisionListener;
	WorldStorage * dataStorage;
	std::list<PhysObjectPtr> physEntities;
	float gravityAcc;

};
