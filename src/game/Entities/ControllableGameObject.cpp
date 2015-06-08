#include "ControllableGameObject.h"

#include "../game.h"
#include "../world/world.h"
#include "../world/blockColumn.h"
#include "../world/worldStorage.h"

ControllableGameObject::ControllableGameObject(ShiftEngine::MeshNode * sceneNode)
    : LiveGameObject(sceneNode)
{
    AddTransition(EntityState::Waiting, EntityState::Rotating);
    AddTransition(EntityState::Rotating, EntityState::Moving);
    AddTransition(EntityState::Waiting, EntityState::Moving);
    AddTransition(EntityState::Rotating, EntityState::Waiting);
    AddTransition(EntityState::Moving, EntityState::Waiting);
    AddTransition(EntityState::Waiting, EntityState::Collecting);
    AddTransition(EntityState::Collecting, EntityState::Waiting);
    AddTransition(EntityState::Waiting, EntityState::Attacking);
    AddTransition(EntityState::Attacking, EntityState::Waiting);
}

ControllableGameObject::~ControllableGameObject()
{
    CancelCommands();
}

bool ControllableGameObject::Go(const MathLib::Vector2F & target)
{
    // check if target position doesn't contain water
    auto pGame = LostIsland::GetGamePtr();
    BlockTypes topColumn = BlockTypes::BT_Empty;

    for (unsigned i = 0;; ++i)
    {
        BlockTypes currentBlock = pGame->world->GetDataStorage()->GetBlockType((int)std::floor(target.x), (int)std::floor(target.y), i++);
        if (currentBlock == BT_Empty)
            break;
        topColumn = currentBlock;
    }

    if (topColumn == BT_Water)
        return false;

    PushCommand(std::make_unique<MoveAction>(target));
    return true;
}

void ControllableGameObject::Stop()
{
    DispatchState(std::make_unique<WaitingState>());
}

void ControllableGameObject::PushCommand(std::unique_ptr<IEntityAction> action)
{
    Actions.push(std::move(action));
}

void ControllableGameObject::CancelCommands()
{
    while (!Actions.empty())
        CancelCurrentCommand();
}

void ControllableGameObject::CancelCurrentCommand()
{
    // HACK IT'S TEMPORARY, WHILE AGGREGATOR IS NOT IMPLEMENTED
    while (!Actions.empty())
    {
        auto action = std::move(Actions.front());
        Actions.pop();
        action->Cancel(this);
    }
}

void ControllableGameObject::Update(double dt)
{
    //may be use stateEnd method inside state?
    if (GetCurrentState()->Dead())
        DispatchState(std::make_unique<WaitingState>());

    if (!Actions.empty())
    {
        auto & action = Actions.front();
        if (!action->IsStarted())
            action->Start(this);

        if (action->IsDead())
            Actions.pop();
    }

    LiveGameObject::Update(dt);
}

void ControllableGameObject::OnStateChange(EntityState from, EntityState to)
{
    if (!Actions.empty())
        Actions.front()->StateChange(this, from, to);
}

void ControllableGameObject::Interact(InteractableGameObject * target, InteractionType interaction)
{
    float radius = 1.0f; // temporarily hardcoded, but must be varied with different interactions
    Vector2F targetPosition = Vector2F(target->GetPosition().x, target->GetPosition().y);
    Vector2F direction = targetPosition - Vector2F(GetPosition().x, GetPosition().y);
    direction = MathLib::normalize(direction);
    targetPosition -= direction * radius;
    switch (interaction)
    {
    case InteractionType::Nothing:
        break;
    case InteractionType::Collecting:
        PushCommand(std::make_unique<MoveAction>(targetPosition));
        PushCommand(std::make_unique<CollectingAction>(3.0, (CollectableGameObject*)target, 3.0f));
        break;
    case InteractionType::Attacking:
        PushCommand(std::make_unique<MoveAction>(targetPosition));
        if (!(LiveGameObject*)target)
            throw std::runtime_error("Unable to cast GameObject into LiveGameObject");
        PushCommand(std::make_unique<AttackAction>((LiveGameObject*)target, 1.0f));
        break;
    default:
        assert(false);
    }
}
