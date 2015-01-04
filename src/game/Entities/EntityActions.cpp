#include "EntityActions.h"

#include "ControllableGameObject.h"
#include "CollectableGameObject.h"

#include "../game.h"

IEntityAction::IEntityAction()
	: m_dead(false)
	, m_started(false)
{
}

IEntityAction::~IEntityAction()
{
}

void IEntityAction::die()
{
	m_dead = true;
}

bool IEntityAction::IsDead() const
{
	return m_dead;
}

bool IEntityAction::IsStarted() const
{
	return m_started;
}

void IEntityAction::Start(ControllableGameObject * gameObject)
{
	if (!gameObject)
		return;

	onStart(gameObject);
	m_started = true;
}

void IEntityAction::End(ControllableGameObject * gameObject)
{
	if (!gameObject)
		return;

	onEnd(gameObject);
	die();
}

void IEntityAction::Cancel(ControllableGameObject * gameObject)
{
	if (!gameObject)
		return;

	onCancel(gameObject);
	die();
}

void IEntityAction::OnStateChange(ControllableGameObject * /*gameObject*/, EntityState /*newState*/)
{
}

//////////////////////////////////////////////////////////////////////////

MoveAction::MoveAction(const MathLib::Vector2F & targetPosition)
	: targetPosition(targetPosition)
	, rotated(false)
{
}

void MoveAction::onStart(ControllableGameObject * gameObject)
{
	// TODO: what if we cannot dispatch it now? postpone?
	gameObject->DispatchState(std::make_unique<RotatingState>(targetPosition));
}

void MoveAction::onEnd(ControllableGameObject * /*gameObject*/)
{
}

void MoveAction::onCancel(ControllableGameObject * gameObject)
{
	gameObject->Stop();
}

void MoveAction::OnStateChange(ControllableGameObject * gameObject, EntityState newState)
{
	if (newState != EntityState::Waiting) // entity stops
		return;

	if (!rotated)
	{
		rotated = true;
		gameObject->DispatchState(std::make_unique<MovingState>(targetPosition));
	}
	else
	{
		this->End(gameObject);
	}
}

//////////////////////////////////////////////////////////////////////////

CollectingAction::CollectingAction(double collecting_time, CollectableGameObject * collectable, float maximum_distance)
	: maximum_distance(maximum_distance)
	, collectable(collectable)
	, collecting_time(collecting_time)
{
}

void CollectingAction::onStart(ControllableGameObject * gameObject)
{
	gameObject->DispatchState(std::make_unique<CollectingState>(collecting_time));
}

void CollectingAction::onEnd(ControllableGameObject * /*gameObject*/)
{
	LostIsland::GetGamePtr()->GlobalEventHandler->onPlayerPicksItem(collectable->GetItemId(), collectable->GetCount());
	collectable->Delete();
}

void CollectingAction::onCancel(ControllableGameObject * gameObject)
{
	gameObject->DispatchState(std::make_unique<WaitingState>());
}

void CollectingAction::OnStateChange(ControllableGameObject * gameObject, EntityState newState)
{
	if (newState == EntityState::Waiting)
		this->End(gameObject);
}

//////////////////////////////////////////////////////////////////////////

void ActionAggregator::OnStateChange(ControllableGameObject * /*gameObject*/, EntityState /*newState*/)
{
}

void ActionAggregator::onStart(ControllableGameObject * /*gameObject*/)
{
}

void ActionAggregator::onEnd(ControllableGameObject * /*gameObject*/)
{
}

void ActionAggregator::onCancel(ControllableGameObject * /*gameObject*/)
{
}

//////////////////////////////////////////////////////////////////////////

AttackAction::AttackAction(LiveGameObject * object, float maximum_distance)
	: target(object)
	, maximum_distance(maximum_distance)
{
}

void AttackAction::onStart(ControllableGameObject * gameObject)
{
	gameObject->DispatchState(std::make_unique<AttackingState>(target, 0.7));
}

void AttackAction::onEnd(ControllableGameObject * gameObject)
{
	gameObject->DispatchState(std::make_unique<WaitingState>());
}

void AttackAction::onCancel(ControllableGameObject * gameObject)
{
	gameObject->DispatchState(std::make_unique<WaitingState>());
}

void AttackAction::OnStateChange(ControllableGameObject * gameObject, EntityState newState)
{
	if (newState != EntityState::Waiting)
		return;

	if (target->GetHealth() > 0) // target is dead
		gameObject->DispatchState(std::make_unique<AttackingState>(target, 0.7));
}
