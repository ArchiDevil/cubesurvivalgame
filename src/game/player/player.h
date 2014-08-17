#pragma once

#include "../../MathLib/math.h"
#include "../world/world.h"
#include "cSelectedBlock.h"
#include "cInventory.h"
#include "../../SimplePhysicsEngine/PhysicsEngine.h"

class cInventory;

struct playerStats
{
	playerStats()
		: hunger(100), health(100), temperature(100)
	{
	}

	unsigned int hunger;
	unsigned int temperature;
	unsigned int health;
};

class cPlayer
{
public:
	cPlayer();
	~cPlayer();

	void Initialize(cWorldStorage * Land);
	void LevelUp();

	//Getters
	unsigned GetHunger() const;
	unsigned GetHealth() const;
	unsigned GetTemperature() const;
	float GetSpeed() const;
	float GetHeight() const;
	Vector3D GetPosition();
	Vector3D * GetVelocitiesPtr();
	cSelectedBlock * GetSelectedBlockPtr();
	cInventory * GetInventoryPtr();
	
	//something
	void FindSelectedBlock(const Vector3D & LOOK);

	//Setters
	void SetPosition(float x, float y, float z);
	void SetPosition(const Vector3D & vec);
	void SetVelocities(const Vector3D & velocities);
	void ResetVelocities();
	void SetHunger(int hunger);
	void SetHealth(int health);
	void SetTemperature(int temperature);

private:
	cWorldStorage * pWorldStorage;
	cSelectedBlock SelectedBlock;
	cInventory * Inventory;
	pPhysObject PlayerBox;
	playerStats Stats;

	float fSpeed;
	float PlayerHeight;

};
