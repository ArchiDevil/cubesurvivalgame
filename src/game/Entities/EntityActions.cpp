#include "EntityActions.h"

#include "LivingGameObject.h"

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
	gameObject->PushState(std::make_shared<MovingState>(targetPosition));
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
