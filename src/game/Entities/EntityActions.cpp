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

void IEntityAction::Update(LivingGameObject * gameObject, double dt)
{
	onUpdate(gameObject, dt);
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

//////////////////////////////////////////////////////////////////////////

RotateAction::RotateAction(const MathLib::Vector2F & targetPosition)
	: targetPosition(targetPosition)
	, rotationTime(0.0)
{
}

void RotateAction::onStart(LivingGameObject * gameObject)
{
	gameObject->SetState(std::make_shared<RotatingState>());
}

void RotateAction::onUpdate(LivingGameObject * gameObject, double dt)
{
	auto * pSceneNode = gameObject->GetSceneNode();
	if (!pSceneNode)
		return;

	rotationTime += dt / 4.0;
	Vector3F thisPos{ pSceneNode->GetPosition().x, pSceneNode->GetPosition().y, 0.0f };
	Vector3F target{ targetPosition.x, targetPosition.y, 0.0 };
	Vector3F faceStart = Vector3F(-1.0f, 0.0f, 0.0f) * pSceneNode->GetRotation();
	Vector3F faceEnd = thisPos - target;
	auto rotation = MathLib::shortest_arc(faceStart, faceEnd);
	if (MathLib::angle(faceStart, faceEnd) <= 0.1f)
	{
		pSceneNode->RotateBy(rotation);
		die();
	}
	else
	{
		auto rotationQuat = MathLib::quaternionSlerp(pSceneNode->GetRotation(), pSceneNode->GetRotation() * rotation, (float)rotationTime);
		pSceneNode->SetRotation(rotationQuat);
	}
}

void RotateAction::onEnd(LivingGameObject * gameObject)
{
	//TODO: use game object calls instead of setting state directly
	gameObject->Stop();
}

void RotateAction::onCancel(LivingGameObject * gameObject)
{
	gameObject->Stop();
}

//////////////////////////////////////////////////////////////////////////

MoveAction::MoveAction(const MathLib::Vector2F & targetPosition)
	: targetPosition(targetPosition)
{
}

void MoveAction::onStart(LivingGameObject * gameObject)
{
	gameObject->SetState(std::make_shared<MovingState>());
}

void MoveAction::onUpdate(LivingGameObject * gameObject, double dt)
{
	auto * pSceneNode = gameObject->GetSceneNode();
	if (!pSceneNode)
		return;

	Vector2F gameObjectPos{ gameObject->GetPosition().x, gameObject->GetPosition().y };

	if (MathLib::distance(gameObjectPos, targetPosition) <= 0.1f)
	{
		pSceneNode->SetPosition(Vector3F(targetPosition.x, targetPosition.y, 0.0f));
		die();
	}
	else
	{
		auto directionVec = MathLib::Normalize(targetPosition - gameObjectPos);
		directionVec *= dt * 5.0f;
		pSceneNode->SetPosition(pSceneNode->GetPosition() + Vector3F(directionVec.x, directionVec.y, 0.0f));
	}
}

void MoveAction::onEnd(LivingGameObject * gameObject)
{
	gameObject->Stop();
}

void MoveAction::onCancel(LivingGameObject * gameObject)
{
	gameObject->Stop();
}

//////////////////////////////////////////////////////////////////////////

DyingAction::DyingAction(double dyingTime)
	: dyingTime(dyingTime)
	, elapsedTime(0.0)
{
}

void DyingAction::onStart(LivingGameObject * gameObject)
{
	gameObject->SetState(std::make_shared<DyingState>());
}

void DyingAction::onUpdate(LivingGameObject * gameObject, double dt)
{
	elapsedTime -= dt;
	auto * pSceneNode = gameObject->GetSceneNode();
	if (!pSceneNode)
		return;

	float scale = elapsedTime / dyingTime;
	pSceneNode->SetScale(scale);
	if (elapsedTime <= 0.0f)
		die();
}

void DyingAction::onEnd(LivingGameObject * gameObject)
{
	// gameObject->AddAction(std::make_shared<DecayAction>());
}

void DyingAction::onCancel(LivingGameObject * gameObject)
{
	throw;
}

//////////////////////////////////////////////////////////////////////////

TimingAction::TimingAction(double activationTime)
	: elapsedTime(activationTime)
{
}

void TimingAction::onUpdate(LivingGameObject * gameObject, double dt)
{
	elapsedTime -= dt;
	if (elapsedTime <= 0.0)
	{
		onEnd(gameObject);
		die();
	}
}

//////////////////////////////////////////////////////////////////////////

CollectingAction::CollectingAction(double time, CollectableGameObject * collectable, float maximum_distance, item_id_t item_id, size_t count)
	: TimingAction(time)
	, maximum_distance(maximum_distance)
	, item_id(item_id)
	, count(count)
	, collectable(collectable)
{
}

void CollectingAction::onStart(LivingGameObject * gameObject)
{
	gameObject->SetState(std::make_shared<CollectingState>());
}

void CollectingAction::onEnd(LivingGameObject * gameObject)
{
	LostIsland::GetGamePtr()->GlobalEventHandler->onPlayerPicksItem(item_id, count);
	collectable->Delete();
}

void CollectingAction::onCancel(LivingGameObject * gameObject)
{
	gameObject->SetState(std::make_shared<WaitingState>());
}
