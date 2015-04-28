#include "PlayerGameObject.h"

#include "../game.h"
#include "../Items/WeaponItem.h"
#include "../world/world.h"
#include "../world/worldStorage.h"
#include "../Environment/EnvironmentManager.h"

#include <GraphicsEngine/ShiftEngine.h>

PlayerGameObject::PlayerGameObject(ShiftEngine::MeshNode * sceneNode, ItemManager * pItemMgr)
    : ControllableGameObject(sceneNode)
    , hunger(25)
    , warmth(100)
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

PlayerInventory * PlayerGameObject::GetInventoryPtr()
{
    return &Inventory;
}

unsigned PlayerGameObject::GetHunger() const
{
    return hunger;
}

unsigned int PlayerGameObject::GetWarmth() const
{
    return warmth;
}

void PlayerGameObject::SetHunger(unsigned int in_hunger)
{
    hunger = in_hunger;
}

void PlayerGameObject::SetWarmth(unsigned int in_temperature)
{
    warmth = in_temperature;
}

void PlayerGameObject::Update(double dt)
{
    auto pGame = LostIsland::GetGamePtr();

    ControllableGameObject::Update(dt);

    if (GetCurrentStateType() != EntityState::Moving &&
        GetCurrentStateType() != EntityState::Rotating)
    {
        targetMarker->SetVisibility(false);
    }

    static double accumulatedTime = 0.0;
    static double prevTime = 0.0;

    prevTime = accumulatedTime;
    accumulatedTime += dt;

    if ((int)accumulatedTime % 10 == 0 && ((int)accumulatedTime - (int)prevTime == 1))
    {
        if (hunger)
            hunger--;
    }

    if ((int)accumulatedTime % 3 == 0 && ((int)accumulatedTime - (int)prevTime == 1))
    {
        if (!hunger)
            SetHealth(GetHealth() - 1);
    }

    if ((int)accumulatedTime % 2 == 0 && ((int)accumulatedTime - (int)prevTime == 1))
    {
        if (warmth < 10)
            SetHealth(GetHealth() - 1);

        if (pGame->environmentMgr->GetEnvironmentTemperature() < 21.0f)
            warmth--;
        else
            warmth++;
    }

    warmth = MathLib::clamp(warmth, 0u, 100u);
}

bool PlayerGameObject::Go(const MathLib::Vector2F & target)
{
    bool result = ControllableGameObject::Go(target);
    if (!result)
        return false;

    auto pGame = LostIsland::GetGamePtr();
    targetMarker->SetVisibility(true);
    targetMarker->SetPosition(Vector3F(target.x, target.y, 0.0f));
    float height = (float)pGame->World->GetDataStorage()->GetFullHeight((int)std::floor(target.x), (int)std::floor(target.y));
    auto position = targetMarker->GetPosition();
    position.z = height;
    targetMarker->SetPosition(position);

    return true;
}

void PlayerGameObject::Attack(LiveGameObject * target) const
{
    if (!target || target->GetHealth() <= 0)
        return;

    auto pGame = LostIsland::GetGamePtr();
    auto itemInHand = pGame->ItemMgr->GetItemById(Inventory.GetItemInRightHand().itemId);

    int damage = 0;

    if (!itemInHand || itemInHand->GetType() != ItemType::Weapon)
        damage = 1;

    if (!damage)
        damage = ((WeaponItem*)itemInHand)->GetDamageCount();

    LOG_INFO("Entity ", target, " damaged with ", damage, ". Remains ", target->GetHealth());
    target->SetHealth(target->GetHealth() - damage);
}

InteractionType PlayerGameObject::GetInteraction() const
{
    return InteractionType::Nothing;
}

void PlayerGameObject::DispatchEvent(const IGameEvent *ev)
{
    if (!ev || ev->GetType() != GE_Heat)
        return;

    HeatEvent* heat = (HeatEvent*)ev;
    int hv = heat->heatValue;

    // assume that campfire heats only 3 blocks around for now
    hv *= (int)(MathLib::distance(heat->heatSource, GetPosition()) / 3.0f);
    SetWarmth(GetWarmth() + hv);
}
