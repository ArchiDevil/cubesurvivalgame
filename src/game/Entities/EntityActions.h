#pragma once

#include <MathLib/math.h>

#include "../Items/ItemManager.h"
#include "EntityState.h"

class LivingGameObject;
class CollectableGameObject;

class IEntityAction
{
public:
	IEntityAction();
	virtual ~IEntityAction();

	void Start(LivingGameObject * gameObject);
	void End(LivingGameObject * gameObject);
	void Cancel(LivingGameObject * gameObject);
	bool IsDead() const;
	bool IsStarted() const;

	virtual void OnStateChange(LivingGameObject * gameObject, EntityState newState);

protected:
	virtual void onStart(LivingGameObject * gameObject) = 0;
	virtual void onEnd(LivingGameObject * gameObject) = 0;
	virtual void onCancel(LivingGameObject * gameObject) = 0;

	void die();

private:
	bool m_dead;
	bool m_started;
};

class MoveAction : public IEntityAction
{
public:
	MoveAction(const MathLib::Vector2F & targetPosition);
	virtual void OnStateChange(LivingGameObject * gameObject, EntityState newState);

protected:
	virtual void onStart(LivingGameObject * gameObject) override;
	virtual void onEnd(LivingGameObject * gameObject) override;
	virtual void onCancel(LivingGameObject * gameObject) override;

private:
	MathLib::Vector2F targetPosition;
	bool rotated;

};

class DyingAction : public IEntityAction
{
public:
	DyingAction(double dyingTime);
	virtual void OnStateChange(LivingGameObject * gameObject, EntityState newState);

protected:
	virtual void onStart(LivingGameObject * gameObject) override;
	virtual void onEnd(LivingGameObject * gameObject) override;
	virtual void onCancel(LivingGameObject * gameObject) override;

private:
	double elapsedTime;
	double dyingTime;

};

class CollectingAction : public IEntityAction
{
public:
	CollectingAction(double collecting_time, CollectableGameObject * collectable, float maximumDistance, item_id_t item_id, size_t count = 1);
	virtual void OnStateChange(LivingGameObject * gameObject, EntityState newState);

protected:
	virtual void onStart(LivingGameObject * gameObject) override;
	virtual void onEnd(LivingGameObject * gameObject) override;
	virtual void onCancel(LivingGameObject * gameObject) override;

private:
	float maximum_distance;	// distance to collect
	item_id_t item_id;
	size_t count;
	CollectableGameObject * collectable;
	double collecting_time;
	
};
