#pragma once

#include "GameObject.h"

class UsableGameObject : public GameObject
{
public:
	UsableGameObject(ShiftEngine::MeshNode * meshNode)
		: GameObject(meshNode) {}
	virtual ~UsableGameObject() {}

	virtual void Activate() = 0;
};
