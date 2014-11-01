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

	if (GetCurrentState() == ES_Moving ||
		GetCurrentState() == ES_Rotating)
	{
		targetMarker->SetVisibility(true);
		targetMarker->SetPosition(Vector3F(targetPosition.x, targetPosition.y, 100.0f));

		auto pGame = LostIsland::GetGamePtr();
		float height = pGame->World->GetDataStorage()->GetFullHeight(std::floor(targetPosition.x), std::floor(targetPosition.y));

		auto position = targetMarker->GetPosition();
		position.z = (float)height;
		targetMarker->SetPosition(position);
	}
	else
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
