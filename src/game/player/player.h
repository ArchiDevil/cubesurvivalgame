#pragma once

#include "../Entities/GameObject.h"
#include "cSelectedBlock.h"

#include <MathLib/math.h>
#include <SimplePhysicsEngine/PhysicsEngine.h>

class cInventory;
class ItemManager;

struct playerStats
{
	playerStats()
		: hunger(100)
		, health(100)
		, temperature(100)
	{}

	unsigned int hunger;
	unsigned int temperature;
	unsigned int health;
};

class PlayerGameObject : public GameObject
{
public:
	PlayerGameObject(ShiftEngine::MeshNode * sceneNode);
	~PlayerGameObject();

	void Initialize(ItemManager * pItemManager);

	//Getters
	unsigned GetHunger() const;
	unsigned GetHealth() const;
	unsigned GetTemperature() const;
	cInventory * GetInventoryPtr();

	void SetHunger(int hunger);
	void SetHealth(int health);
	void SetTemperature(int temperature);

	virtual void Update(double dt);

private:
	std::unique_ptr<cInventory> Inventory;
	playerStats Stats;

};
