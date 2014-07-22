#pragma once

#include <list>

#include "../game/world/cWorldStorage.h"
#include "PhysicsFunctions.h"
#include "types.h"
#include "CollisionShouter.h"

#include "../Utilities/singleton.h"

class cWorldStorage;

class cSimplePhysicsEngine : public singleton<cSimplePhysicsEngine>		//may be define it as global?
{
public:
	cSimplePhysicsEngine();
	~cSimplePhysicsEngine();

	void Initialize(cWorldStorage * storage, double _Gravity = -9.81f);
	void Update( double dt = 0.0f );

	pPhysObject GetPlayerBBox();
	pPhysObject CreateEntity(Vector3D & Position, Vector3D & Velocity);

	void ChangePlayerFreeState();			//позволяет игроку летать
	bool IsPlayerFree() const;				//возвращает возможность полета для игрока
	bool IsPlayerCollidesWithWorld() const;
	int GetPhysEntSize() const;
	std::vector<CollisionInfo> & GetCollisionsPlayerEntity();

private:
	void UpdatePlayer( double dt );
	void UpdateEntities( double dt );

	pPhysObject PlayerBBox;
	CollisionListener cl;

	cWorldStorage * WorldStorage;
	bool FreeMode;
	bool PlayerCollidesWithWorld;
	double Gravity;

	std::list<pPhysObject> PhysEntities;

};