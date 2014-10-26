#include "PlayerGameObject.h"
#include "../cInventory.h"
#include "../game.h"
#include <GraphicsEngine/ShiftEngine.h>

PlayerGameObject::PlayerGameObject(ShiftEngine::MeshNode * sceneNode)
	: LivingGameObject(sceneNode)
	, hunger(100)
	, health(100)
	, temperature(100)
	, targetMarker(nullptr)
{
}

PlayerGameObject::~PlayerGameObject() 
{
}

void PlayerGameObject::Initialize()
{
	Inventory.reset(new cInventory(LostIsland::GetGamePtr()->ItemMgr));
	if (targetMarker)
		targetMarker->KillSelf();

	auto * pScene = ShiftEngine::GetSceneGraph();
	auto material = ShiftEngine::GetContextManager()->LoadMaterial(L"player.mtl", L"playerTargetMarker");
	targetMarker = pScene->AddMeshNode(ShiftEngine::Utilities::createCube(), MathLib::AABB(Vector3F(-0.5f, -0.5f, 0.0f), Vector3F(0.5f, 0.5f, 1.0f)), material.get());
	targetMarker->SetVisibility(false);
	targetMarker->SetScale(0.3f);
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
