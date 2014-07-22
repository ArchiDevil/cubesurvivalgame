#pragma once

#include "GameObject.h"
#include "../SimplePhysicsEngine/types.h"

using ShiftEngine::MeshNode;

class PhysicsEntity : public GameObject
{
	friend class cEntityManager;

public:
	PhysicsEntity(pPhysObject _obj, MeshNode * meshNode);
	virtual ~PhysicsEntity();

	virtual void Update(double dt);
	virtual void SetPosition(Vector3D & position);

private:
	pPhysObject obj;

};