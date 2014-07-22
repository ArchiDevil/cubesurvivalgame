#include "PhysicsEntity.h"

PhysicsEntity::PhysicsEntity(pPhysObject _obj, ShiftEngine::MeshNode * meshNode)
	: GameObject(meshNode), obj(_obj)
{
}

PhysicsEntity::~PhysicsEntity() 
{
}

void PhysicsEntity::Update( double /*dt*/ )
{
	SceneNode->SetPosition(Vector3F(obj.GetPtr()->Position.x, obj.GetPtr()->Position.y, obj.GetPtr()->Position.z));
}

void PhysicsEntity::SetPosition( Vector3D & position )
{
	obj.GetPtr()->Position = position;
}
