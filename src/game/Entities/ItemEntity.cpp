#include "ItemEntity.h"

ItemEntity::ItemEntity(uint64_t itemId, pPhysObject _obj, ShiftEngine::MeshNode * meshNode)
	: itemId(itemId)
	, PhysicsEntity(_obj, meshNode)
{
}

ItemEntity::~ItemEntity()
{
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

uint64_t ItemEntity::GetItemId() const
{
	return itemId;
}
