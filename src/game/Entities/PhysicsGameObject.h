#pragma once

#include <SimplePhysicsEngine/types.h>
#include "GameObject.h"

class PhysicsGameObject : public GameObject
{
public:
    PhysicsGameObject(PhysObjectPtr physicsObject, ShiftEngine::MeshNode * meshNode);
    virtual ~PhysicsGameObject();

    virtual void Update(double dt);
    virtual void SetPosition(MathLib::Vector3F & position);

private:
    PhysObjectPtr physicsObject;

};

typedef std::shared_ptr<PhysicsGameObject> PhysicsEntityPtr;
