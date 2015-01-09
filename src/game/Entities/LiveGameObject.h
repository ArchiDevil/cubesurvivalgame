#pragma once

#include "InteractableGameObject.h"
#include "../FiniteStateMachine.h"

// Live game objects are objects that can be damaged
// and have their own state (like die, attack and anything)

class LiveGameObject : public InteractableGameObject, public FiniteStateMachine
{
public:
	LiveGameObject(ShiftEngine::MeshNode * sceneNode);
	virtual ~LiveGameObject();

	virtual void Update(double dt) override;
	virtual void Attack(LiveGameObject * target) const;

	// getters
	int GetHealth() const;
	virtual InteractionType GetInteraction() const override;
	GameObjectInventory * GetInventory();

	// setters
	void SetHealth(int in_health);

private:
	int health;
	GameObjectInventory inventory;

};
