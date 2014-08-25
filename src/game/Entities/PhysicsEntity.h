#pragma once

#include <SimplePhysicsEngine/types.h>

#include "GameObject.h"

using ShiftEngine::MeshNode;

class PhysicsEntity : public GameObject
{
public:
	PhysicsEntity(pPhysObject _obj, MeshNode * meshNode);
	virtual ~PhysicsEntity();

	virtual void Update(double dt);
	virtual void SetPosition(Vector3D & position);

private:
	pPhysObject obj;

};
