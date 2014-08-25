#pragma once

#include "UsableGameObject.h"
#include "../Items/Item.h"

class CrafterGameObject : public UsableGameObject
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

	// UsableGameObject
	virtual void Activate() override;

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
	Item * storedItem;
};
