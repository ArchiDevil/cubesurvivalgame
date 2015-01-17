#include "PhysicsEngine.h"

#include <string>

#include <game/world/worldStorage.h>
#include <Utilities/logger.hpp>

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
		float l = 0.0f;
		float r = (float)dt;
		float eps = 1e-6f;
		Vector3F newPos = object->Position;
		while (fabs(l - r) > eps)
		{
			float cur_dt = (l + r) / 2.0f;

			Vector3F Velocities = object->Velocities;
			Velocities.z += gravityAcc * cur_dt;
			Vector3F delta = Velocities * (float)cur_dt;
			newPos = object->Position + delta;
			AABB bboxToCalculate = { object->bbox.bMin + newPos, object->bbox.bMax + newPos };

			if (!Physics::IsAABBCollidesWithWorld(bboxToCalculate, dataStorage))
				l = cur_dt;
			else
				r = cur_dt;
		}
		object->Velocities.z += gravityAcc * r;
		object->Position = newPos;
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
