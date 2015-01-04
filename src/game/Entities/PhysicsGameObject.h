#pragma once

#include <SimplePhysicsEngine/types.h>
#include "GameObject.h"

using ShiftEngine::MeshNode;

class PhysicsGameObject : public GameObject
{
public:
	PhysicsGameObject(PhysObjectPtr physicsObject, MeshNode * meshNode);
	virtual ~PhysicsGameObject();

	virtual void Update(double dt);
	virtual void SetPosition(MathLib::Vector3F & position);

private:
	PhysObjectPtr physicsObject;

};

typedef std::shared_ptr<PhysicsGameObject> PhysicsEntityPtr;
