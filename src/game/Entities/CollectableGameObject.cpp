#include "CollectableGameObject.h"

#include "../game.h"

CollectableGameObject::CollectableGameObject(ShiftEngine::MeshNode * sceneNode, item_id_t itemId)
	: itemId(itemId)
	, StaticGameObject(sceneNode)
{
}

CollectableGameObject::~CollectableGameObject()
{
}

std::unique_ptr<IEntityAction> CollectableGameObject::GetInteraction()
{
	return std::make_unique<CollectingAction>(2.0, this, itemId);
}
