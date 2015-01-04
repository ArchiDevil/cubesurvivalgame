#pragma once

#include "../Items/ItemManager.h"
#include "EntityState.h"

#include <MathLib/math.h>

#include <memory>

class ControllableGameObject;
class CollectableGameObject;
class LiveGameObject;

class IEntityAction
{
public:
	IEntityAction();
	virtual ~IEntityAction();

	void Start(ControllableGameObject * gameObject);
	void End(ControllableGameObject * gameObject);
	void Cancel(ControllableGameObject * gameObject);
	bool IsDead() const;
	bool IsStarted() const;

	virtual void OnStateChange(ControllableGameObject * gameObject, EntityState newState);

protected:
	virtual void onStart(ControllableGameObject * gameObject) = 0;
	virtual void onEnd(ControllableGameObject * gameObject) = 0;
	virtual void onCancel(ControllableGameObject * gameObject) = 0;

	void die();

private:
	bool m_dead;
	bool m_started;
};

// should be used for aggregating actions like a: move (rotate + move), attack (move + attack) and etc.
class ActionAggregator : public IEntityAction
{
public:
	ActionAggregator() = delete;
	ActionAggregator(const ActionAggregator&) = delete;
	ActionAggregator(ActionAggregator&&) = delete;
	ActionAggregator& operator=(const ActionAggregator&) = delete;
	ActionAggregator& operator=(ActionAggregator&&) = delete;
	virtual void OnStateChange(ControllableGameObject * gameObject, EntityState newState) override;

protected:
	ActionAggregator(std::initializer_list<std::unique_ptr<IEntityAction>> init_list);
	virtual void onStart(ControllableGameObject * gameObject) override;
	virtual void onEnd(ControllableGameObject * gameObject) override;
	virtual void onCancel(ControllableGameObject * gameObject) override;

};

class MoveAction : public IEntityAction
{
public:
	MoveAction(const MathLib::Vector2F & targetPosition);
	virtual void OnStateChange(ControllableGameObject * gameObject, EntityState newState);

protected:
	virtual void onStart(ControllableGameObject * gameObject) override;
	virtual void onEnd(ControllableGameObject * gameObject) override;
	virtual void onCancel(ControllableGameObject * gameObject) override;

private:
	MathLib::Vector2F targetPosition;
	bool rotated;

};

class CollectingAction : public IEntityAction
{
public:
	CollectingAction(double collecting_time, CollectableGameObject * collectable, float maximumDistance);
	virtual void OnStateChange(ControllableGameObject * gameObject, EntityState newState);

protected:
	virtual void onStart(ControllableGameObject * gameObject) override;
	virtual void onEnd(ControllableGameObject * gameObject) override;
	virtual void onCancel(ControllableGameObject * gameObject) override;

private:
	float maximum_distance;	// distance to collect
	CollectableGameObject * collectable;
	double collecting_time;

};

class AttackAction : public IEntityAction
{
public:
	AttackAction(LiveGameObject * object, float maximum_distance);
	virtual void OnStateChange(ControllableGameObject * gameObject, EntityState newState);

protected:
	virtual void onStart(ControllableGameObject * gameObject) override;
	virtual void onEnd(ControllableGameObject * gameObject) override;
	virtual void onCancel(ControllableGameObject * gameObject) override;

	LiveGameObject * target;
	float maximum_distance;

};
