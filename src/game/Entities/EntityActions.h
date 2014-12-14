#pragma once

#include <MathLib/math.h>

#include "../Items/ItemManager.h"

class LivingGameObject;
class CollectableGameObject;

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

class DyingAction : public IEntityAction
{
public:
	DyingAction(double dyingTime);

protected:
	virtual void onStart(LivingGameObject * gameObject) override;
	virtual void onUpdate(LivingGameObject * gameObject, double dt) override;
	virtual void onEnd(LivingGameObject * gameObject) override;
	virtual void onCancel(LivingGameObject * gameObject) override;

private:
	double elapsedTime;
	double dyingTime;

};

class TimingAction : public IEntityAction
{
public:
	TimingAction(double activationTime);

protected:
	virtual void onUpdate(LivingGameObject * gameObject, double dt) override;

private:
	double elapsedTime;
};

class CollectingAction : public TimingAction
{
public:
	CollectingAction(double time, CollectableGameObject * collectable, float maximumDistance, item_id_t item_id, size_t count = 1);

protected:
	virtual void onStart(LivingGameObject * gameObject);
	virtual void onEnd(LivingGameObject * gameObject);
	virtual void onCancel(LivingGameObject * gameObject);

private:
	float maximum_distance;	// distance to collect
	item_id_t item_id;
	size_t count;
	CollectableGameObject * collectable;
	
};
