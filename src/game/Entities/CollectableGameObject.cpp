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
