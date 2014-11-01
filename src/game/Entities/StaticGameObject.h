#pragma once

#include "GameObject.h"

// Static game objects are objects that cannot be moved
// they cannot do anything except of receiving damage and die
// e.g. trees, rocks and any resource-collectible objects

class StaticGameObject : public GameObject
{
public:
	StaticGameObject(ShiftEngine::MeshNode * mesh)
		: GameObject(mesh)
	{
	}

	~StaticGameObject()
	{
	}

	virtual void Update(double dt)
	{
	}
};
