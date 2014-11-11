#include "EntityState.h"
#include "GameObject.h"

IEntityState::IEntityState()
	: dead(false)
{
}

IEntityState::~IEntityState()
{
}

bool IEntityState::Dead() const
{
	return dead;
}

void IEntityState::Die()
{
	dead = true;
}

//////////////////////////////////////////////////////////////////////////

RotatingState::RotatingState(const MathLib::Vector2F & targetPosition)
	: targetPosition(targetPosition)
{
}

void RotatingState::Update(GameObject * entity, double dt)
{
	auto * pSceneNode = entity->GetSceneNode();
	if (!pSceneNode)
		return;
	static double rotationTime = 0.0;
	rotationTime += dt / 4.0;
	Vector3F thisPos = pSceneNode->GetPosition();
	thisPos.z = 0.0f;
	Vector3F target = Vector3F(targetPosition.x, targetPosition.y, 0.0);
	Vector3F faceStart = Vector3F(-1.0f, 0.0f, 0.0f) * pSceneNode->GetRotation();
	Vector3F faceEnd = thisPos - target;
	auto rotation = MathLib::shortest_arc(faceStart, faceEnd);
	if (MathLib::angle(faceStart, faceEnd) <= 0.1f)
	{
		pSceneNode->RotateBy(rotation);
		Die();
	}
	else
	{
		auto rotationQuat = MathLib::quaternionSlerp(pSceneNode->GetRotation(), pSceneNode->GetRotation() * rotation, (float)rotationTime);
		pSceneNode->SetRotation(rotationQuat);
	}
}

EntityState RotatingState::GetType() const
{
	return EntityState::Rotating;
}

//////////////////////////////////////////////////////////////////////////

MovingState::MovingState(const MathLib::Vector2F & targetPosition)
	: targetPosition(targetPosition)
{
}

void MovingState::Update(GameObject * entity, double dt)
{
	auto * pSceneNode = entity->GetSceneNode();
	if (!pSceneNode)
		return;
	Vector3F currentPos = pSceneNode->GetPosition();
	currentPos.z = 0.0f;
	Vector3F targetPos = Vector3F(targetPosition.x, targetPosition.y, 0.0f);
	if (MathLib::distance(currentPos, targetPos) <= 0.1f)
	{
		pSceneNode->SetPosition(targetPos);
		Die();
	}
	else
	{
		auto directionVec = MathLib::Normalize(targetPos - currentPos);
		directionVec *= dt * 5.0f;
		pSceneNode->SetPosition(pSceneNode->GetPosition() + directionVec);
	}
}

EntityState MovingState::GetType() const
{
	return EntityState::Moving;
}

//////////////////////////////////////////////////////////////////////////
