#include "PhysicsGameObject.h"

PhysicsGameObject::PhysicsGameObject(PhysObjectPtr obj, ShiftEngine::MeshNode * meshNode)
	: GameObject(meshNode), obj(obj)
{
}

PhysicsGameObject::~PhysicsGameObject()
{
}

void PhysicsGameObject::Update(double /*dt*/)
{
	SceneNode->SetPosition(Vector3F(obj->Position.x, obj->Position.y, obj->Position.z));
}

void PhysicsGameObject::SetPosition(MathLib::Vector3F & position)
{
	obj->Position = position;
}
