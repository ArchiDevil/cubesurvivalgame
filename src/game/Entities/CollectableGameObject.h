#pragma once

#include "StaticGameObject.h"

class CollectableGameObject : public StaticGameObject
{
public:
	CollectableGameObject(ShiftEngine::MeshNode * sceneNode, uint32_t itemId);
	~CollectableGameObject();

private:
	uint32_t itemId;

};
