#pragma once

#include <MathLib/math.h>

class GameObject;

enum class EntityState
{
	Waiting,
	Rotating,
	Moving,
};

class IEntityState
{
public:
	IEntityState();
    virtual ~IEntityState();

	virtual void Update(GameObject * entity, double dt) = 0;
	virtual bool Dead() const;
	virtual EntityState GetType() const = 0;

protected:
	void Die();

	bool dead;
};

class RotatingState : public IEntityState
{
public:
	RotatingState(const MathLib::Vector2F & targetPosition);
	virtual void Update(GameObject * entity, double dt) override;
	virtual EntityState GetType() const override;

private:
	const MathLib::Vector2F targetPosition;
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

	//ES_Attacking,
	//ES_Dying
