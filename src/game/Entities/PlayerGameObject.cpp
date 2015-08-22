#include "PlayerGameObject.h"

#include "../game.h"
#include "../Items/WeaponItem.h"
#include "../world/world.h"
#include "../world/worldStorage.h"
#include "../Environment/EnvironmentManager.h"

#include <GraphicsEngine/ShiftEngine.h>

PlayerGameObject::PlayerGameObject(ShiftEngine::MeshNode * sceneNode, ItemManager * pItemMgr)
    : ControllableGameObject(sceneNode)
    , targetMarker(nullptr)
    , Inventory(pItemMgr, 10)
{
    AddTransition(EntityState::ToolUsing, EntityState::Waiting);
    AddTransition(EntityState::Waiting, EntityState::ToolUsing);
    AddTransition(EntityState::Moving, EntityState::ToolUsing);
    AddTransition(EntityState::Rotating, EntityState::ToolUsing);

    auto * pScene = ShiftEngine::GetSceneGraph();
    auto material = ShiftEngine::GetContextManager()->LoadMaterial(L"player.mtl", L"playerTargetMarker");
    targetMarker = pScene->AddMeshNode(ShiftEngine::Utilities::createCube(), material.get());
    targetMarker->SetVisibility(false);
    targetMarker->SetScale(0.3f);

    SetMaxHealth(20);
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
    if (hunger > maxHunger)
        hunger = maxHunger;
}

void PlayerGameObject::SetWarmth(unsigned int in_temperature)
{
    warmth = in_temperature;
    if (warmth > maxWarmth)
        warmth = maxWarmth;
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
            SetHunger(hunger - 1);
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
            SetWarmth(warmth - 1);
        else
            SetWarmth(warmth + 1);
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
    float height = (float)pGame->world->GetDataStorage()->GetFullHeight((int)std::floor(target.x), (int)std::floor(target.y));
    targetMarker->SetPosition({ target.x, target.y, height });

    return true;
}

void PlayerGameObject::Attack(LiveGameObject * target) const
{
    if (!target || target->GetHealth() <= 0)
        return;

    auto pGame = LostIsland::GetGamePtr();
    auto itemInHand = pGame->itemMgr->GetItemById(Inventory.GetItemInRightHand().itemId);

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

unsigned int PlayerGameObject::GetMaxHunger() const
{
    return maxHunger;
}

unsigned int PlayerGameObject::GetMaxWarmth() const
{
    return maxWarmth;
}

void PlayerGameObject::SetMaxHunger(unsigned int val)
{
    maxHunger = val;
}

void PlayerGameObject::Fishing(const MathLib::Vector3F & targetBlock) const
{
    // player goes to the some point near water and fish here
}
