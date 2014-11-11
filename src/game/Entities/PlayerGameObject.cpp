#include "PlayerGameObject.h"
#include "../game.h"
#include <GraphicsEngine/ShiftEngine.h>

PlayerGameObject::PlayerGameObject(ShiftEngine::MeshNode * sceneNode, ItemManager * pItemMgr)
	: LivingGameObject(sceneNode)
	, hunger(100)
	, temperature(100)
	, targetMarker(nullptr)
	, Inventory(pItemMgr)
{
	if (targetMarker)
		targetMarker->KillSelf();

	auto * pScene = ShiftEngine::GetSceneGraph();
	auto material = ShiftEngine::GetContextManager()->LoadMaterial(L"player.mtl", L"playerTargetMarker");
	targetMarker = pScene->AddMeshNode(ShiftEngine::Utilities::createCube(), MathLib::AABB(Vector3F(-0.5f, -0.5f, 0.0f), Vector3F(0.5f, 0.5f, 1.0f)), material.get());
	targetMarker->SetVisibility(false);
	targetMarker->SetScale(0.3f);
}

PlayerGameObject::~PlayerGameObject() 
{
}

cInventory * PlayerGameObject::GetInventoryPtr()
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

void PlayerGameObject::SetHunger(int hunger)
{
	this->hunger = hunger;
}

void PlayerGameObject::SetTemperature(int temperature)
{
	this->temperature = temperature;
}

void PlayerGameObject::Update(double dt)
{
	LivingGameObject::Update(dt);

	if (GetCurrentState() != EntityState::Moving &&
		GetCurrentState() != EntityState::Rotating)
	{
		targetMarker->SetVisibility(false);
	}

	static double accumulatedTime = 0.0;
	accumulatedTime += dt;
	if ((int)accumulatedTime % 10 == 0)
	{
		hunger--;
		accumulatedTime = 1.0;
	}
}

bool PlayerGameObject::Go(const MathLib::Vector2F & target)
{
	bool result = LivingGameObject::Go(target);
	if (!result) return false;

	auto pGame = LostIsland::GetGamePtr();
	targetMarker->SetVisibility(true);
	targetMarker->SetPosition(Vector3F(target.x, target.y, 0.0f));
	float height = pGame->World->GetDataStorage()->GetFullHeight(std::floor(target.x), std::floor(target.y));
	auto position = targetMarker->GetPosition();
	position.z = (float)height;
	targetMarker->SetPosition(position);

	return true;
}
