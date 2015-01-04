#include "EntityState.h"

#include "GameObject.h"
#include "ControllableGameObject.h"

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

WaitingState::WaitingState()
{
}

void WaitingState::Update(LiveGameObject * /*entity*/, double /*dt*/)
{
}

EntityState WaitingState::GetType() const
{
	return EntityState::Waiting;
}

//////////////////////////////////////////////////////////////////////////

RotatingState::RotatingState(const MathLib::Vector2F & targetPosition)
	: targetPosition(targetPosition)
	, rotatingTime(0.0)
{
}

void RotatingState::Update(LiveGameObject * entity, double dt)
{
	auto * pSceneNode = entity->GetSceneNode();
	if (!pSceneNode)
		return;
	rotatingTime += dt / 4.0;
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
		auto rotationQuat = MathLib::quaternionSlerp(pSceneNode->GetRotation(), pSceneNode->GetRotation() * rotation, (float)rotatingTime);
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

void MovingState::Update(LiveGameObject * entity, double dt)
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

DyingState::DyingState(double dyingTime)
	: accumulatedTime(0.0)
	, fullTime(dyingTime)
{
}

void DyingState::Update(LiveGameObject * entity, double dt)
{
	//HACK: with big FPS there's will be an error due to cast
	accumulatedTime += dt;
	auto * pSceneNode = entity->GetSceneNode();
	if (!pSceneNode)
		return;

	float scale = (fullTime - accumulatedTime) / fullTime;
	pSceneNode->SetScale(scale);
	if (accumulatedTime > fullTime)
		entity->DispatchState(std::make_unique<DecayState>(2.0));
}

EntityState DyingState::GetType() const
{
	return EntityState::Dying;
}

//////////////////////////////////////////////////////////////////////////

DecayState::DecayState(float decayTime)
	: elapsedTime(decayTime)
{
}

void DecayState::Update(LiveGameObject * entity, double dt)
{
	//HACK: with big FPS there's will be an error due to cast
	elapsedTime -= (float)dt;
	if (elapsedTime <= 0.0f)
		entity->Delete();
}

EntityState DecayState::GetType() const
{
	return EntityState::Decay;
}

//////////////////////////////////////////////////////////////////////////

CollectingState::CollectingState(double collectingTime)
	: collectingTime(collectingTime)
	, accumulatedTime(0.0)
{
}

EntityState CollectingState::GetType() const
{
	return EntityState::Collecting;
}

void CollectingState::Update(LiveGameObject * entity, double dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= collectingTime)
	{
		entity->DispatchState(std::make_unique<WaitingState>());
		// switch to waiting or previous
	}
}

//////////////////////////////////////////////////////////////////////////

AttackingState::AttackingState(LiveGameObject * target, double cycleTime)
	: target(target)
	, cycleTime(cycleTime)
	, accumulatedTime(0.0)
{
}

EntityState AttackingState::GetType() const
{
	return EntityState::Attacking;
}

void AttackingState::Update(LiveGameObject * entity, double dt)
{
	if (!target)
		return;
	
	accumulatedTime += dt;
	if (accumulatedTime >= cycleTime)
	{
		// tick
		accumulatedTime -= cycleTime;
		entity->Attack(target);
		entity->DispatchState(std::make_unique<WaitingState>());
	}
}
