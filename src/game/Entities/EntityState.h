#pragma once

#include <MathLib/math.h>

class GameObject;

enum class EntityState
{
	Waiting,
	Rotating,
	Moving,
	Dying,
	Decay,
	Collecting
};

class IEntityState
{
public:
	IEntityState();
	virtual ~IEntityState();

	virtual bool Dead() const;
	virtual EntityState GetType() const = 0;

protected:
	void Die();

	bool dead;
};

class WaitingState : public IEntityState
{
public:
	WaitingState();
	virtual EntityState GetType() const override;
};

class RotatingState : public IEntityState
{
public:
	RotatingState();
	virtual EntityState GetType() const override;
};

class MovingState : public IEntityState
{
public:
	MovingState();
	virtual EntityState GetType() const override;
};

class DyingState : public IEntityState
{
public:
	DyingState();
	virtual EntityState GetType() const override;
};

class DecayState : public IEntityState
{
public:
	DecayState();
	virtual EntityState GetType() const override;
};

class CollectingState : public IEntityState
{
public:
	CollectingState();
	virtual EntityState GetType() const override;
};

//ES_Attacking,
