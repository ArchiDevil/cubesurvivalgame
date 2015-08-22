#include "PhysicsGameObject.h"

PhysicsGameObject::PhysicsGameObject(PhysObjectPtr physicsObject, ShiftEngine::MeshNode * meshNode)
    : GameObject(meshNode)
    , physicsObject(physicsObject)
{
}

PhysicsGameObject::~PhysicsGameObject()
{
}

void PhysicsGameObject::Update(double /*dt*/)
{
    sceneNode->SetPosition(physicsObject->Position);
}

void PhysicsGameObject::SetPosition(MathLib::Vector3F & position)
{
    physicsObject->Position = position;
}
