#pragma once

#include "GameObject.h"

class DamagableGameObject : public GameObject
{
public:
	DamagableGameObject(ShiftEngine::MeshNode * meshNode, int health)
		: GameObject(meshNode), health(health)
	{}
	virtual ~DamagableGameObject() {}

	int GetHealth() const { return health; }
	void SetHealth(int _health) { health = _health; }

private:
	int health;

};
