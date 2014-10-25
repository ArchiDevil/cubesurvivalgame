#pragma once

#include "LivingGameObject.h"

#include <MathLib/math.h>

class cInventory;

class PlayerGameObject : public LivingGameObject
{
public:
	PlayerGameObject(ShiftEngine::MeshNode * sceneNode);
	~PlayerGameObject();

	void Initialize();

	//Getters
	unsigned GetHunger() const;
	unsigned GetHealth() const;
	unsigned GetTemperature() const;
	cInventory * GetInventoryPtr();

	void SetHunger(int hunger);
	void SetHealth(int health);
	void SetTemperature(int temperature);

private:
	std::unique_ptr<cInventory> Inventory;

	unsigned int hunger;
	unsigned int temperature;
	unsigned int health;

};
