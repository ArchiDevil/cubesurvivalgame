#include "EntityActions.h"

#include "LivingGameObject.h"
#include "CollectableGameObject.h"

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

void IEntityAction::Start(LivingGameObject * gameObject)
{
	onStart(gameObject);
	m_started = true;
}

void IEntityAction::End(LivingGameObject * gameObject)
{
	onEnd(gameObject);
	die();
}

void IEntityAction::Cancel(LivingGameObject * gameObject)
{
	onCancel(gameObject);
	die();
}

void IEntityAction::OnStateChange(LivingGameObject * /*gameObject*/, EntityState /*newState*/)
{
}

//////////////////////////////////////////////////////////////////////////

MoveAction::MoveAction(const MathLib::Vector2F & targetPosition)
	: targetPosition(targetPosition)
	, rotated(false)
{
}

void MoveAction::onStart(LivingGameObject * gameObject)
{
	// TODO: what if we cannot dispatch it now? postpone?
	gameObject->DispatchState(std::make_unique<RotatingState>(targetPosition));
}

void MoveAction::onEnd(LivingGameObject * /*gameObject*/)
{
}

void MoveAction::onCancel(LivingGameObject * gameObject)
{
	gameObject->Stop();
}

void MoveAction::OnStateChange(LivingGameObject * gameObject, EntityState newState)
{
	if (newState == EntityState::Waiting) // entity stops
	{
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
}

//////////////////////////////////////////////////////////////////////////

DyingAction::DyingAction(double dyingTime)
	: dyingTime(dyingTime)
	, elapsedTime(0.0)
{
}

void DyingAction::onStart(LivingGameObject * gameObject)
{
	gameObject->DispatchState(std::make_unique<DyingState>(dyingTime));
}

void DyingAction::onEnd(LivingGameObject * /*gameObject*/)
{
}

void DyingAction::onCancel(LivingGameObject * /*gameObject*/)
{
	assert(false); // this state couldn't be cancelled
}

void DyingAction::OnStateChange(LivingGameObject * gameObject, EntityState /*newState*/)
{
	End(gameObject);
}

//////////////////////////////////////////////////////////////////////////

CollectingAction::CollectingAction(double collecting_time, CollectableGameObject * collectable, float maximum_distance, item_id_t item_id, size_t count)
	: maximum_distance(maximum_distance)
	, item_id(item_id)
	, count(count)
	, collectable(collectable)
	, collecting_time(collecting_time)
{
}

void CollectingAction::onStart(LivingGameObject * gameObject)
{
	gameObject->DispatchState(std::make_unique<CollectingState>(collecting_time));
}

void CollectingAction::onEnd(LivingGameObject * /*gameObject*/)
{
	LostIsland::GetGamePtr()->GlobalEventHandler->onPlayerPicksItem(item_id, count);
	collectable->Delete();
}

void CollectingAction::onCancel(LivingGameObject * gameObject)
{
	gameObject->DispatchState(std::make_unique<WaitingState>());
}

void CollectingAction::OnStateChange(LivingGameObject * gameObject, EntityState newState)
{
	if (newState == EntityState::Waiting)
	{
		this->End(gameObject);
	}
}
