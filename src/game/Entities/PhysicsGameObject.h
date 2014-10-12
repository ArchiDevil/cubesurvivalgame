#pragma once

#include <SimplePhysicsEngine/types.h>

#include "GameObject.h"

using ShiftEngine::MeshNode;

class PhysicsGameObject : public GameObject
{
public:
	PhysicsGameObject(pPhysObject _obj, MeshNode * meshNode);
	virtual ~PhysicsGameObject();

	virtual void Update(double dt);
	virtual void SetPosition(Vector3D & position);

private:
	pPhysObject obj;

};
