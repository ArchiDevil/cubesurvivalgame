#pragma once

#include "ControllableGameObject.h"
#include "../Entities/GameObjectInventory.h"

#include <MathLib/math.h>

class PlayerGameObject : public ControllableGameObject
{
public:
	PlayerGameObject(ShiftEngine::MeshNode * sceneNode, ItemManager * pItemMgr);
	~PlayerGameObject();

	//Getters
	unsigned GetHunger() const;
	unsigned GetTemperature() const;
	PlayerInventory * GetInventoryPtr();
	void SetHunger(int hunger);
	void SetTemperature(int temperature);
	void Update(double dt) override;
	bool Go(const MathLib::Vector2F & target) override;
	void Attack(LiveGameObject * target) const override;
	InteractionType GetInteraction() const override;

private:
	PlayerInventory Inventory;

	unsigned int hunger;
	unsigned int temperature;

	ShiftEngine::MeshNode * targetMarker;

};

typedef std::shared_ptr<PlayerGameObject> PlayerPtr;
