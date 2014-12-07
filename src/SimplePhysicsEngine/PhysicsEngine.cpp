#include "PhysicsEngine.h"

#include <game/world/worldStorage.h>

#include <string>
#include <Utilities/logger.h>

SimplePhysicsEngine::SimplePhysicsEngine()
	: dataStorage(nullptr)
	, gravityAcc(-9.81f)
{
}

SimplePhysicsEngine::~SimplePhysicsEngine()
{
}

void SimplePhysicsEngine::Initialize(WorldStorage * storage, float gravity)
{
	dataStorage = storage;
	gravityAcc = gravity;
}

void SimplePhysicsEngine::Update(double dt)
{
	collisionListener.ClearCollisions();
	UpdatePhysics(dt);
}

PhysObjectPtr SimplePhysicsEngine::CreateEntity(const Vector3F & position, const Vector3F & velocity, const AABB & bbox)
{
	PhysObjectPtr out(new PhysObject(position, velocity, bbox));
	physEntities.push_back(out);
	return out;
}

void SimplePhysicsEngine::UpdatePhysics(double dt)
{
	for (auto &object : physEntities)
	{
		object->Velocities.z += gravityAcc * (float)dt;
		MathLib::Vector3F delta = object->Velocities * (float)dt;
		MathLib::Vector3F newPos = object->Position + delta;
		MathLib::AABB bboxToCalculate = { object->bbox.bMin + newPos, object->bbox.bMax + newPos };

		if (!Physics::IsAABBCollidesWithWorld(bboxToCalculate, dataStorage))
		{
			object->Position = newPos;
		}
		else
		{
			// wrong on big speed!
			object->Velocities.z = 0.0f;
		}
	}
}

/*
void cSimplePhysicsEngine::UpdatePlayer( double dt )
{
	Vector3D XNew = Vector3D(PlayerBBox.GetPtr()->Velocities.x * dt, 0.0f, 0.0f);
	Vector3D YNew = Vector3D(0.0f, PlayerBBox.GetPtr()->Velocities.y * dt, 0.0f);
	Vector3D ZNew = Vector3D(0.0f, 0.0f, PlayerBBox.GetPtr()->Velocities.z * dt);

	if(!Physics::IsPlayerCollidesWithWorld(PlayerBBox.GetPtr()->Position + XNew, 1.6f, WorldStorage))
	{
	PlayerBBox.GetPtr()->Position += XNew;
	}

	if(!Physics::IsPlayerCollidesWithWorld(PlayerBBox.GetPtr()->Position + YNew, 1.6f, WorldStorage))
	{
	PlayerBBox.GetPtr()->Position += YNew;
	}

	if(!Physics::IsPlayerCollidesWithWorld(PlayerBBox.GetPtr()->Position + ZNew, 1.6f, WorldStorage))
	{
	PlayerBBox.GetPtr()->Position += ZNew;
	PlayerCollidesWithWorld = false;
	}
	else
	{
	PlayerBBox.GetPtr()->Velocities.z = 0.0f;
	PlayerCollidesWithWorld = true;
	}

	PlayerBBox.GetPtr()->Velocities.x = 0.0f;
	PlayerBBox.GetPtr()->Velocities.y = 0.0f;
}
*/

const std::vector<CollisionInfo> & SimplePhysicsEngine::GetCollisions() const
{
	return collisionListener.GetCollisionsInfo();
}

int SimplePhysicsEngine::GetPhysEntitiesCount() const
{
	return physEntities.size();
}
