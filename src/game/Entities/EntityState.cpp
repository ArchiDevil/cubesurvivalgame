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

WaitingState::WaitingState()
{
}

EntityState WaitingState::GetType() const
{
	return EntityState::Waiting;
}

//////////////////////////////////////////////////////////////////////////

RotatingState::RotatingState()
{
}

EntityState RotatingState::GetType() const
{
	return EntityState::Rotating;
}

//////////////////////////////////////////////////////////////////////////

MovingState::MovingState()
{
}

EntityState MovingState::GetType() const
{
	return EntityState::Moving;
}

//////////////////////////////////////////////////////////////////////////

DyingState::DyingState()
{
}

EntityState DyingState::GetType() const
{
	return EntityState::Dying;
}

//////////////////////////////////////////////////////////////////////////

DecayState::DecayState()
{
}

EntityState DecayState::GetType() const
{
	return EntityState::Decay;
}

//////////////////////////////////////////////////////////////////////////

CollectingState::CollectingState()
{
}

EntityState CollectingState::GetType() const
{
	return EntityState::Collecting;
}
