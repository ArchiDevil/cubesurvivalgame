#pragma once

#include "../../MathLib/math.h"
#include "../world/world.h"
#include "cSelectedBlock.h"
#include "cInventory.h"
#include "../../SimplePhysicsEngine/PhysicsEngine.h"

class cInventory;

struct playerRPGStats
{
	playerRPGStats()
		: iHunger(100), iHealth(100), iLongFatique(100), iFastFatique(100), iMana(100),	iWeight(70), 
		iStrength(10), iAgility(10), iStamina(10), iIntelligency(10), iDefense(0), iAttack(0),
		sLevel(1), iExperience(0), sProfCoint(0), sStatsCoint(0)
	{};

	unsigned int iHunger;
	unsigned int iHealth;
	unsigned int iLongFatique;
	unsigned int iFastFatique;
	unsigned int iMana;
	unsigned int iWeight;			//вес игрока?
	unsigned int iStrength;
	unsigned int iAgility;
	unsigned int iStamina;
	unsigned int iIntelligency;
	unsigned int iDefense;
	unsigned int iAttack;

	unsigned int sLevel;			//уровень героя
	unsigned int iExperience;		//опыт 
	unsigned int sProfCoint, sStatsCoint;
};

class cPlayer
{
public:
	cPlayer();
	~cPlayer();

	void Initialize(cWorldStorage * Land);
	void LevelUp();

	//Getters
	int GetHunger() const;
	int GetHealth() const;
	int GetLongFatique() const;
	int GetFastFatique() const;
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
	void SetHunger(int hungerLevel);
	void SetHealth(int healthLevel);
	void SetLongFatique(int fatiqueLevel);
	void SetFastFatique(int fatiqueLevel);

private:
	void UpdateStats();								//updating RPG stats after level up

	cWorldStorage * pWorldStorage;
	cSelectedBlock SelectedBlock;
	cInventory * Inventory;
	pPhysObject PlayerBox;
	playerRPGStats Stats;

	float fSpeed;
	float PlayerHeight;

};