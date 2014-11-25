#pragma once

#include <MathLib/math.h>

class LivingGameObject;

class IEntityAction
{
public:
	IEntityAction();
	virtual ~IEntityAction();

	virtual void OnStart(LivingGameObject * gameObject) = 0;
	virtual void OnUpdate(LivingGameObject * gameObject, double dt) = 0;
	virtual void OnEnd(LivingGameObject * gameObject) = 0;
	virtual void OnCancel(LivingGameObject * gameObject) = 0;
	bool IsDead() const;

protected:
	void die();

private:
	bool m_dead;
};

class MoveAction : public IEntityAction
{
public:
	MoveAction(const MathLib::Vector2F & targetPosition);

	virtual void OnStart(LivingGameObject * gameObject) override;
	virtual void OnUpdate(LivingGameObject * gameObject, double dt) override;
	virtual void OnEnd(LivingGameObject * gameObject) override;
	virtual void OnCancel(LivingGameObject * gameObject) override;

private:
	MathLib::Vector2F targetPosition;

};
