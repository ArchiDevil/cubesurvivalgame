#include "player.h"
#include "cInventory.h"

#include "../game.h"

const float BlockRadius = 4.0f;	//��������� ������ ����� �������� ����

PlayerGameObject::PlayerGameObject(ShiftEngine::MeshNode * sceneNode)
	: GameObject(sceneNode)
{
}

PlayerGameObject::~PlayerGameObject() 
{
}

void PlayerGameObject::Initialize( ItemManager * pItemManager )
{
	Inventory.reset(new cInventory(pItemManager));
}

cInventory * PlayerGameObject::GetInventoryPtr()
{
	return Inventory.get();
}

unsigned PlayerGameObject::GetHunger() const
{
	return Stats.hunger;
}

unsigned PlayerGameObject::GetHealth() const
{
	return Stats.health;
}

unsigned PlayerGameObject::GetTemperature() const
{
	return Stats.temperature;
}

void PlayerGameObject::SetHunger(int hunger)
{
	Stats.hunger = hunger;
}

void PlayerGameObject::SetHealth(int health)
{
	Stats.health = health;
}

void PlayerGameObject::SetTemperature(int temperature)
{
	Stats.temperature = temperature;
}

void PlayerGameObject::Update(double dt)
{
	GameObject::Update(dt);

	auto pGame = LostIsland::GetGamePtr();
	Vector3F position = this->GetPosition();
	auto height = pGame->World->GetDataStorage()->GetFullHeight(position.x, position.y);
	position.z = (float)height;
	this->SetPosition(position);
}
