#include "EntityActions.h"

#include "LivingGameObject.h"

IEntityAction::IEntityAction()
	: m_dead(false)
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

//////////////////////////////////////////////////////////////////////////

MoveAction::MoveAction(const MathLib::Vector2F & targetPosition)
	: IEntityAction()
	, targetPosition(targetPosition)
{
}

void MoveAction::OnStart(LivingGameObject * gameObject)
{
	gameObject->PushState(std::make_shared<MovingState>(targetPosition));
}

void MoveAction::OnUpdate(LivingGameObject * gameObject, double dt)
{
	auto * pSceneNode = gameObject->GetSceneNode();
	if (!pSceneNode)
		return;

	Vector2F gameObjectPos = { gameObject->GetPosition().x, gameObject->GetPosition().y };

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

void MoveAction::OnEnd(LivingGameObject * gameObject)
{
	gameObject->Stop();
}

void MoveAction::OnCancel(LivingGameObject * gameObject)
{
	gameObject->Stop();
}
