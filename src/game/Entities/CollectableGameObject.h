#pragma once

#include "StaticGameObject.h"

#include "../Items/ItemManager.h"

class CollectableGameObject : public StaticGameObject
{
public:
	CollectableGameObject(ShiftEngine::MeshNode * sceneNode, item_id_t itemId);
	~CollectableGameObject();

private:
	item_id_t itemId;

};