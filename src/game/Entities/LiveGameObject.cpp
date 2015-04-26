#include "LiveGameObject.h"

#include "../game.h"
#include "../world/world.h"
#include "../world/worldStorage.h"

LiveGameObject::LiveGameObject(ShiftEngine::MeshNode * sceneNode)
    : InteractableGameObject(sceneNode)
    , health(1)
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

    if (health <= 0)
        DispatchState(std::make_unique<DyingState>(1.0));
}

void LiveGameObject::Update(double dt)
{
    FiniteStateMachine::GetCurrentState()->Update(this, dt);

    auto pGame = LostIsland::GetGamePtr();
    auto bbox = sceneNode->GetBBox();
    int heights[4] = { 0 };
    int minX = (int)std::floor(bbox.bMin.x);
    int maxX = (int)std::floor(bbox.bMax.x);
    int minY = (int)std::floor(bbox.bMin.y);
    int maxY = (int)std::floor(bbox.bMax.y);
    heights[0] = pGame->world->GetDataStorage()->GetFullHeight(minX, minY);
    heights[1] = pGame->world->GetDataStorage()->GetFullHeight(minX, maxY);
    heights[2] = pGame->world->GetDataStorage()->GetFullHeight(maxX, minY);
    heights[3] = pGame->world->GetDataStorage()->GetFullHeight(maxX, maxY);

    float maxHeight = (float)heights[0];
    for (int i = 0; i < 4; ++i)
        if (maxHeight < heights[i])
            maxHeight = (float)heights[i];

    auto position = GetPosition();
    position.z = (float)maxHeight;
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
