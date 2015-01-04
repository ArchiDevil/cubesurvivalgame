#include "PlayerGameObject.h"
#include "../game.h"
#include "../Items/WeaponItem.h"

#include <GraphicsEngine/ShiftEngine.h>

PlayerGameObject::PlayerGameObject(ShiftEngine::MeshNode * sceneNode, ItemManager * pItemMgr)
	: ControllableGameObject(sceneNode)
	, hunger(25)
	, temperature(100)
	, targetMarker(nullptr)
	, Inventory(pItemMgr, 10)
{
	auto * pScene = ShiftEngine::GetSceneGraph();
	auto material = ShiftEngine::GetContextManager()->LoadMaterial(L"player.mtl", L"playerTargetMarker");
	targetMarker = pScene->AddMeshNode(ShiftEngine::Utilities::createCube(), MathLib::AABB(Vector3F(-0.5f, -0.5f, 0.0f), Vector3F(0.5f, 0.5f, 1.0f)), material.get());
	targetMarker->SetVisibility(false);
	targetMarker->SetScale(0.3f);

	SetHealth(20);
}

PlayerGameObject::~PlayerGameObject() 
{
}

PlayerInventory * PlayerGameObject::GetInventoryPtr()
{
	return &Inventory;
}

unsigned PlayerGameObject::GetHunger() const
{
	return hunger;
}

unsigned PlayerGameObject::GetTemperature() const
{
	return temperature;
}

void PlayerGameObject::SetHunger(int in_hunger)
{
	this->hunger = in_hunger;
}

void PlayerGameObject::SetTemperature(int in_temperature)
{
	this->temperature = in_temperature;
}

void PlayerGameObject::Update(double dt)
{
	ControllableGameObject::Update(dt);

	if (GetCurrentStateType() != EntityState::Moving &&
		GetCurrentStateType() != EntityState::Rotating)
	{
		targetMarker->SetVisibility(false);
	}

	static double accumulatedTime = 0.0;
	accumulatedTime += dt;
	if ((int)accumulatedTime % 10 == 0)
	{
		if (hunger)
			hunger--;
		if (!hunger)
			SetHealth(GetHealth() - 1);
		accumulatedTime = 1.0;
	}
}

bool PlayerGameObject::Go(const MathLib::Vector2F & target)
{
	bool result = ControllableGameObject::Go(target);
	if (!result) 
		return false;

	auto pGame = LostIsland::GetGamePtr();
	targetMarker->SetVisibility(true);
	targetMarker->SetPosition(Vector3F(target.x, target.y, 0.0f));
	float height = pGame->World->GetDataStorage()->GetFullHeight(std::floor(target.x), std::floor(target.y));
	auto position = targetMarker->GetPosition();
	position.z = (float)height;
	targetMarker->SetPosition(position);

	return true;
}

void PlayerGameObject::Attack(LiveGameObject * target) const
{
	auto pGame = LostIsland::GetGamePtr();
	auto itemInHand = pGame->ItemMgr->GetItemById(Inventory.GetItemInRightHand().itemId);

	if (!itemInHand)
		return;

	if (itemInHand->GetType() == IT_Weapon)
	{
		LOG_INFO("Entity " + std::to_string((size_t)target) + " damaged with " + std::to_string(((WeaponItem*)itemInHand)->GetDamageCount()) + ". Remains " + std::to_string(target->GetHealth()));
		target->SetHealth(target->GetHealth() - ((WeaponItem*)itemInHand)->GetDamageCount());
	}
	else
	{
		LOG_INFO("Entity " + std::to_string((size_t)target) + " damaged with " + std::to_string(1) + ". Remains " + std::to_string(target->GetHealth()));
		target->SetHealth(target->GetHealth() - 1);
	}
}

InteractionType PlayerGameObject::GetInteraction() const
{
	return InteractionType::Nothing;
}
