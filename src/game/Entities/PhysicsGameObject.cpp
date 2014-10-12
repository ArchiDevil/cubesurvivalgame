#include "PhysicsGameObject.h"

PhysicsGameObject::PhysicsGameObject(pPhysObject _obj, ShiftEngine::MeshNode * meshNode)
	: GameObject(meshNode), obj(_obj)
{
}

PhysicsGameObject::~PhysicsGameObject() 
{
}

void PhysicsGameObject::Update( double /*dt*/ )
{
	SceneNode->SetPosition(Vector3F(obj.GetPtr()->Position.x, obj.GetPtr()->Position.y, obj.GetPtr()->Position.z));
}

void PhysicsGameObject::SetPosition( Vector3D & position )
{
	obj.GetPtr()->Position = position;
}
