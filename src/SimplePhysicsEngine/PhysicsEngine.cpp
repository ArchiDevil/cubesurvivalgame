#include "PhysicsEngine.h"

cSimplePhysicsEngine::cSimplePhysicsEngine()
	: dataStorage(nullptr)
	, gravityAcc(-9.81f)
{
}

cSimplePhysicsEngine::~cSimplePhysicsEngine()
{
}

void cSimplePhysicsEngine::Initialize(WorldStorage * storage, float gravity)
{
	dataStorage = storage;
	gravityAcc = gravity;
}

void cSimplePhysicsEngine::Update(double dt)
{
	collisionListener.ClearCollisions();
	UpdatePhysics(dt);
}

PhysObjectPtr cSimplePhysicsEngine::CreateEntity(const Vector3F & Position, const Vector3F & Velocity)
{
	PhysObjectPtr out(new PhysObject(Position, Velocity));
	physEntities.push_back(out);
	return out;
}

void cSimplePhysicsEngine::UpdatePhysics(double dt)
{
	//Vector3D XNew = Vector3D(PlayerBBox.GetPtr()->Velocities.x * dt, 0.0f, 0.0f);
	//Vector3D YNew = Vector3D(0.0f, PlayerBBox.GetPtr()->Velocities.y * dt, 0.0f);
	//Vector3D ZNew = Vector3D(0.0f, 0.0f, PlayerBBox.GetPtr()->Velocities.z * dt);

	//if(!FreeMode)
	//{
	//	//if(!Physics::IsPlayerCollidesWithWorld(PlayerBBox.GetPtr()->Position + XNew, 1.6f, WorldStorage))
	//	{
	//		PlayerBBox.GetPtr()->Position += XNew;
	//	}

	//	//if(!Physics::IsPlayerCollidesWithWorld(PlayerBBox.GetPtr()->Position + YNew, 1.6f, WorldStorage))
	//	{
	//		PlayerBBox.GetPtr()->Position += YNew;
	//	}

	//	//if(!Physics::IsPlayerCollidesWithWorld(PlayerBBox.GetPtr()->Position + ZNew, 1.6f, WorldStorage))
	//	{
	//		PlayerBBox.GetPtr()->Position += ZNew;
	//		PlayerCollidesWithWorld = false;
	//	}
	//	//else
	//	{
	//		PlayerBBox.GetPtr()->Velocities.z = 0.0f;
	//		PlayerCollidesWithWorld = true;
	//	}
	//}
	//else
	//{
	//	PlayerBBox.GetPtr()->Position = PlayerBBox.GetPtr()->Position + XNew + YNew + ZNew;
	//}

	//if(FreeMode)
	//{
	//	PlayerBBox.GetPtr()->Velocities = Vector3D();	//null
	//}

	//PlayerBBox.GetPtr()->Velocities.x = 0.0f;
	//PlayerBBox.GetPtr()->Velocities.y = 0.0f;
}

std::vector<CollisionInfo> & cSimplePhysicsEngine::GetCollisionsPlayerEntity()
{
	return collisionListener.GetCollisionsPlayerEntityInfo();
}

int cSimplePhysicsEngine::GetPhysEntitySize() const
{
	return physEntities.size();
}
