#pragma once

#include <SimplePhysicsEngine/types.h>

#include "GameObject.h"

using ShiftEngine::MeshNode;

class PhysicsGameObject : public GameObject
{
public:
	PhysicsGameObject(PhysObjectPtr obj, MeshNode * meshNode);
	virtual ~PhysicsGameObject();

	virtual void Update(double dt);
	virtual void SetPosition(MathLib::Vector3F & position);

private:
	PhysObjectPtr obj;

};
