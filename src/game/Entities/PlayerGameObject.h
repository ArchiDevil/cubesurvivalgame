#pragma once

#include "LivingGameObject.h"
#include "../cInventory.h"

#include <MathLib/math.h>

class PlayerGameObject : public LivingGameObject
{
public:
	PlayerGameObject(ShiftEngine::MeshNode * sceneNode, ItemManager * pItemMgr);
	~PlayerGameObject();

	//Getters
	unsigned GetHunger() const;
	unsigned GetTemperature() const;
	cInventory * GetInventoryPtr();

	void SetHunger(int hunger);
	void SetTemperature(int temperature);

	void Update(double dt) override;

	virtual bool Go(const MathLib::Vector2F & target) override;

private:
	cInventory Inventory;

	unsigned int hunger;
	unsigned int temperature;

	ShiftEngine::MeshNode * targetMarker;

};

typedef std::shared_ptr<PlayerGameObject> PlayerPtr;
