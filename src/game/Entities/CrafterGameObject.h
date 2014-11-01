#pragma once

#include "GameObject.h"
#include "../Items/Item.h"

class CrafterGameObject : public GameObject
{
	enum CrafterState
	{
		Empty,
		Crafting,
		Done
	};

public:
	CrafterGameObject(ShiftEngine::MeshNode * meshNode, uint32_t craftingTimeMs);
	virtual ~CrafterGameObject();
	virtual void Activate();

	// GameObject
	virtual void Update(double dt) override;

private:
	//crafter must implement crafting mechanics such as:
	//when player activates it, crafter must take item from player's hand
	//and place it inside himself
	//after some time, crafter must replace one item to another (e.g. raw fish -> cooked fish)
	//and player should be available to take item from crafter

	CrafterState state;
	const uint32_t craftingTime;
	uint32_t elapsedTime;
	uint64_t storedItem;
};

typedef std::shared_ptr<CrafterGameObject> CrafterPtr;
