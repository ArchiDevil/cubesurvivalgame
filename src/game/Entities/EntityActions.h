#pragma once

#include <MathLib/math.h>

class LivingGameObject;

class IEntityAction
{
public:
	IEntityAction();
	virtual ~IEntityAction();

	void Start(LivingGameObject * gameObject);
	void Update(LivingGameObject * gameObject, double dt);
	void End(LivingGameObject * gameObject);
	void Cancel(LivingGameObject * gameObject);
	bool IsDead() const;
	bool IsStarted() const;

protected:
	virtual void onStart(LivingGameObject * gameObject) = 0;
	virtual void onUpdate(LivingGameObject * gameObject, double dt) = 0;
	virtual void onEnd(LivingGameObject * gameObject) = 0;
	virtual void onCancel(LivingGameObject * gameObject) = 0;

	void die();

private:
	bool m_dead;
	bool m_started;
};

class RotateAction : public IEntityAction
{
public:
	RotateAction(const MathLib::Vector2F & targetPosition);

protected:
	virtual void onStart(LivingGameObject * gameObject) override;
	virtual void onUpdate(LivingGameObject * gameObject, double dt) override;
	virtual void onEnd(LivingGameObject * gameObject) override;
	virtual void onCancel(LivingGameObject * gameObject) override;

private:
	MathLib::Vector2F targetPosition;
	double rotationTime;
};

class MoveAction : public IEntityAction
{
public:
	MoveAction(const MathLib::Vector2F & targetPosition);

protected:
	virtual void onStart(LivingGameObject * gameObject) override;
	virtual void onUpdate(LivingGameObject * gameObject, double dt) override;
	virtual void onEnd(LivingGameObject * gameObject) override;
	virtual void onCancel(LivingGameObject * gameObject) override;

private:
	MathLib::Vector2F targetPosition;

};
