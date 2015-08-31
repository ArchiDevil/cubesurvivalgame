#include "LiveGameObject.h"

#include "../game.h"
#include "../world/world.h"
#include "../world/worldStorage.h"

LiveGameObject::LiveGameObject(ShiftEngine::MeshNode * sceneNode)
    : InteractableGameObject(sceneNode)
    , inventory(LostIsland::GetGamePtr()->itemMgr.get(), 10)
{
    FiniteStateMachine::AddTransition(EntityState::Waiting, EntityState::Dying);
    FiniteStateMachine::AddTransition(EntityState::Dying, EntityState::Decay);
}

LiveGameObject::~LiveGameObject()
{
}

int LiveGameObject::GetHealth() const
{
    return health;
}

void LiveGameObject::SetHealth(int in_health)
{
    health = in_health;
    if (health > maxHealth)
        health = maxHealth;

    if (health <= 0)
        DispatchState(std::make_unique<DyingState>(1.0));
}

void LiveGameObject::Update(double dt)
{
    FiniteStateMachine::GetCurrentState()->Update(this, dt);

    auto pDataStorage = LostIsland::GetGamePtr()->world->GetDataStorage();

    auto position = GetPosition();

    int topX = (int)std::floor(position.x + 0.5f);
    int topY = (int)std::floor(position.y + 0.5f);
    int bottomX = (int)std::floor(position.x - 0.5f);
    int bottomY = (int)std::floor(position.y - 0.5f);

    float k = position.x - bottomX - 0.5f;
    float v1 = MathLib::LinearInterpolation<float>((float)pDataStorage->GetFullHeight(bottomX, topY), (float)pDataStorage->GetFullHeight(topX, topY), k);
    float v2 = MathLib::LinearInterpolation<float>((float)pDataStorage->GetFullHeight(bottomX, bottomY), (float)pDataStorage->GetFullHeight(topX, bottomY), k);

    k = position.y - bottomY - 0.5f;
    float v3 = MathLib::LinearInterpolation(v1, v2, k);

    position.z = v3;
    SetPosition(position);
}

InteractionType LiveGameObject::GetInteraction() const
{
    // check this
    return InteractionType::Attacking;
}

GameObjectInventory * LiveGameObject::GetInventory()
{
    return &inventory;
}

void LiveGameObject::Attack(LiveGameObject * /*target*/) const
{
}

int LiveGameObject::GetMaxHealth() const
{
    return maxHealth;
}

void LiveGameObject::SetMaxHealth(int in_maxHealth)
{
    maxHealth = in_maxHealth;
}
