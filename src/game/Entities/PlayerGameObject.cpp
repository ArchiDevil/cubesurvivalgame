#include "PlayerGameObject.h"
#include "../cInventory.h"
#include "../game.h"

const float BlockRadius = 4.0f;	//насколько далеко можно выделять блок

PlayerGameObject::PlayerGameObject(ShiftEngine::MeshNode * sceneNode)
	: LivingGameObject(sceneNode)
	, hunger(100)
	, health(100)
	, temperature(100)
{
}

PlayerGameObject::~PlayerGameObject() 
{
}

void PlayerGameObject::Initialize()
{
	Inventory.reset(new cInventory(LostIsland::GetGamePtr()->ItemMgr));
}

cInventory * PlayerGameObject::GetInventoryPtr()
{
	return Inventory.get();
}

unsigned PlayerGameObject::GetHunger() const
{
	return hunger;
}

unsigned PlayerGameObject::GetHealth() const
{
	return health;
}

unsigned PlayerGameObject::GetTemperature() const
{
	return temperature;
}

void PlayerGameObject::SetHunger(int hunger)
{
	hunger = hunger;
}

void PlayerGameObject::SetHealth(int health)
{
	health = health;
}

void PlayerGameObject::SetTemperature(int temperature)
{
	temperature = temperature;
}
