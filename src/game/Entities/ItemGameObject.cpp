#include "ItemGameObject.h"

#include "../game.h"

ItemGameObject::ItemGameObject(uint64_t itemId, size_t count, PhysObjectPtr _obj, ShiftEngine::MeshNode * meshNode) 
	: itemId(itemId)
	, count(count)
	, PhysicsGameObject(_obj, meshNode)
{
}

ItemGameObject::~ItemGameObject()
{
}

void ItemGameObject::Update(double dt)
{
	PhysicsGameObject::Update(dt);

	//	Vector3F rot = SceneNode->GetRotation();
	//	rot.z += dt;
	//	SceneNode->SetRotation(rot);

	cGame * pGame = LostIsland::GetGamePtr();
	auto ppos = pGame->Player->GetPosition();
	if (MathLib::distance((Vector3F)ppos, GetPosition()) < 1.0f)
	{
		pGame->GlobalEventHandler->onPlayerPicksItem(itemId, count);
		Delete();
	}
}

uint64_t ItemGameObject::GetItemId() const
{
	return itemId;
}
