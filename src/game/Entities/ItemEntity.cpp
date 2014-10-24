#include "ItemEntity.h"

#include "../game.h"
#include "../player/cInventory.h"

ItemEntity::ItemEntity(uint64_t itemId, pPhysObject _obj, ShiftEngine::MeshNode * meshNode)
	: itemId(itemId)
	, PhysicsGameObject(_obj, meshNode)
{
}

ItemEntity::~ItemEntity()
{
}

void ItemEntity::Update( double dt )
{
	PhysicsGameObject::Update(dt);

//	Vector3F rot = SceneNode->GetRotation();
//	rot.z += dt;
//	SceneNode->SetRotation(rot);

	cGame * pGame = LostIsland::GetGamePtr();
	auto ppos = pGame->Player->GetPosition();
	if (MathLib::distance((Vector3F)ppos, GetPosition()) < 1.0f)
	{
		pGame->Player->GetInventoryPtr()->GetHandPtr()->itemId = itemId;
		pGame->Player->GetInventoryPtr()->GetHandPtr()->count++;
		this->Delete();
	}
}

uint64_t ItemEntity::GetItemId() const
{
	return itemId;
}
