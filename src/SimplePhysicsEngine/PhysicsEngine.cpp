#include "PhysicsEngine.h"

using MathLib::Vector3D;

cSimplePhysicsEngine::cSimplePhysicsEngine() 
	: WorldStorage(nullptr), FreeMode(false), 
	Gravity(-9.81f), PlayerBBox(new PhysObject),
	PlayerCollidesWithWorld(false)
{
}

cSimplePhysicsEngine::~cSimplePhysicsEngine()
{
}

void cSimplePhysicsEngine::Initialize( cWorldStorage * storage, double _Gravity )
{
	WorldStorage = storage;
	Gravity = _Gravity;
}

void cSimplePhysicsEngine::Update( double dt /*= 0.0f*/ )
{
	cl.ClearCollisions();
	UpdatePlayer(dt);
	UpdateEntities(dt);
}

void cSimplePhysicsEngine::ChangePlayerFreeState()
{
	if(FreeMode)
		FreeMode = false;
	else
		FreeMode = true;
}

bool cSimplePhysicsEngine::IsPlayerFree() const
{
	return FreeMode;
}

pPhysObject cSimplePhysicsEngine::CreateEntity( Vector3D & Position, Vector3D & Velocity )
{
	pPhysObject out(new PhysObject(Position, Velocity));
	PhysEntities.push_back(out);
	return out;
}

void cSimplePhysicsEngine::UpdatePlayer( double dt )
{
	if(!FreeMode)
	{
		PlayerBBox.GetPtr()->Velocities.z += Gravity * dt;
	}

	Vector3D XNew = Vector3D(PlayerBBox.GetPtr()->Velocities.x * dt, 0.0f, 0.0f);
	Vector3D YNew = Vector3D(0.0f, PlayerBBox.GetPtr()->Velocities.y * dt, 0.0f);
	Vector3D ZNew = Vector3D(0.0f, 0.0f, PlayerBBox.GetPtr()->Velocities.z * dt);

	if(!FreeMode)
	{
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
	}
	else
	{
		PlayerBBox.GetPtr()->Position = PlayerBBox.GetPtr()->Position + XNew + YNew + ZNew;
	}

	if(FreeMode)
	{
		PlayerBBox.GetPtr()->Velocities = Vector3D();	//null
	}

	//	if(PlayerCollidesWithWorld)
	//	{
	PlayerBBox.GetPtr()->Velocities.x = 0.0f;
	PlayerBBox.GetPtr()->Velocities.y = 0.0f;
	//	}
}

void cSimplePhysicsEngine::UpdateEntities( double dt )
{
	for(auto iter = PhysEntities.begin(); iter != PhysEntities.end(); iter++)
	{
		if((*iter).GetPtr()->getUsings() == 1)
		{
			iter = PhysEntities.erase(iter);
			break;
		}

		pPhysObject obj = (*iter);
		//working with object, we need to calculate it's new position at X, Y, Z
		//no-no, only at Z position
		//we think that our entitles cannot move in horizontal direction

		obj.GetPtr()->Velocities.z += Gravity * dt;

		Vector3D NewZ = Vector3D(0.0f, 0.0f, obj.GetPtr()->Velocities.z * dt);

		if(!Physics::IsPointCollidesWithWorld(obj.GetPtr()->Position + NewZ, WorldStorage))
		{
			obj.GetPtr()->Position += NewZ;
		}
		else
		{
			obj.GetPtr()->Velocities.z = 0.0f;
		}

		if(Physics::IsPlayerCollidesWithBlock(PlayerBBox.GetPtr()->Position, obj.GetPtr()->Position, 1.6f))	//HACK: hardCODE!
		{
			cl.AddCollision(CollisionInfo(obj));
		}
	}
	//we don't collect ENT->WORLD collisions, only ENT->PLAYER
}

std::vector<CollisionInfo> & cSimplePhysicsEngine::GetCollisionsPlayerEntity()
{
	return cl.GetCollisionsPlayerEntityInfo();
}

int cSimplePhysicsEngine::GetPhysEntSize() const
{
	return PhysEntities.size();
}

pPhysObject cSimplePhysicsEngine::GetPlayerBBox()
{
	return PlayerBBox;
}

bool cSimplePhysicsEngine::IsPlayerCollidesWithWorld() const
{
	return PlayerCollidesWithWorld;
}
