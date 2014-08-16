#include "ItemEntity.h"

ItemEntity::ItemEntity( Item * item, pPhysObject _obj, ShiftEngine::MeshNode * meshNode )
	: item(item), PhysicsEntity(_obj, meshNode)
{
}

ItemEntity::~ItemEntity()
{
}

Item * ItemEntity::GetItemPtr() const
{
	return item;
}

void ItemEntity::Update( double dt )
{
	PhysicsEntity::Update(dt);

	Vector3F rot = SceneNode->GetRotation();
	rot.z += dt;
	SceneNode->SetRotation(rot);

//	D3DXVECTOR3 pos = Mesh.GetPosition();
//	pos.z += 0.1f;
//	Mesh.SetPosition(pos);
}
