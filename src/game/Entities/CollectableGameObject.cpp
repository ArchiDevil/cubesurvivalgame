#include "CollectableGameObject.h"

#include "../game.h"

CollectableGameObject::CollectableGameObject(ShiftEngine::MeshNode * sceneNode, uint32_t itemId)
	: itemId(itemId)
	, StaticGameObject(sceneNode)
{
}

CollectableGameObject::~CollectableGameObject()
{
}
