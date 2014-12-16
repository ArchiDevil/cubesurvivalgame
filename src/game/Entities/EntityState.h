#pragma once

#include <MathLib/math.h>

#include <map>

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
	virtual void Update(GameObject * entity, double dt) = 0;
	bool DispatchState(EntityState state) const;

protected:
	void AddTransition(EntityState from, EntityState to);
	void Die();

	std::multimap<EntityState, EntityState> transitions;
	bool dead;
};

class WaitingState : public IEntityState
{
public:
	WaitingState();
	virtual void Update(GameObject * entity, double dt) override;
	virtual EntityState GetType() const override;
};

class RotatingState : public IEntityState
{
public:
	RotatingState(const MathLib::Vector2F & targetPosition);
	virtual void Update(GameObject * entity, double dt) override;
	virtual EntityState GetType() const override;

private:
	const MathLib::Vector2F targetPosition;
	double rotatingTime;
};

class MovingState : public IEntityState
{
public:
	MovingState(const MathLib::Vector2F & targetPosition);
	virtual void Update(GameObject * entity, double dt) override;
	virtual EntityState GetType() const override;

private:
	MathLib::Vector2F targetPosition;

};

class DyingState : public IEntityState
{
public:
	DyingState(float dyingTime);
	virtual void Update(GameObject * entity, double dt) override;
	virtual EntityState GetType() const override;

private:
	float elapsedTime;
	const float fullTime;

};

class DecayState : public IEntityState
{
public:
	DecayState(float decayTime);
	virtual void Update(GameObject * entity, double dt) override;
	virtual EntityState GetType() const override;

private:
	float elapsedTime;
};

class CollectingState : public IEntityState
{
public:
	CollectingState(double collectingTime);
	virtual EntityState GetType() const override;
	virtual void Update(GameObject * entity, double dt) override;

private:
	double collectingTime;
	double accumulatedTime;

};

//ES_Attacking,
