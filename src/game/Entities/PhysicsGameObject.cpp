#include "PhysicsGameObject.h"

PhysicsGameObject::PhysicsGameObject(PhysObjectPtr physicsObject, ShiftEngine::MeshNode * meshNode)
	: GameObject(meshNode), physicsObject(physicsObject)
{
}

PhysicsGameObject::~PhysicsGameObject()
{
}

void PhysicsGameObject::Update(double /*dt*/)
{
	SceneNode->SetPosition(Vector3F(physicsObject->Position.x, physicsObject->Position.y, physicsObject->Position.z));
}

void PhysicsGameObject::SetPosition(MathLib::Vector3F & position)
{
	physicsObject->Position = position;
}
