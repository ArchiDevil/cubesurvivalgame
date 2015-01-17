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
	void StateChange(ControllableGameObject * gameObject, EntityState oldState, EntityState newState);
	bool IsDead() const;
	bool IsStarted() const;
	bool IsCancelled() const;

protected:
	virtual void onStart(ControllableGameObject * gameObject) = 0;
	virtual void onEnd(ControllableGameObject * gameObject) = 0;
	virtual void onCancel(ControllableGameObject * gameObject) = 0;
	virtual void onStateChange(ControllableGameObject * gameObject, EntityState oldState, EntityState newState) = 0;

	void die();

private:
	bool m_dead;
	bool m_started;
	bool m_cancelled;
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

protected:
	ActionAggregator(std::initializer_list<std::unique_ptr<IEntityAction>> init_list);
	virtual void onStart(ControllableGameObject * gameObject) override;
	virtual void onEnd(ControllableGameObject * gameObject) override;
	virtual void onCancel(ControllableGameObject * gameObject) override;
	virtual void onStateChange(ControllableGameObject * gameObject, EntityState oldState, EntityState newState) override;

};

class MoveAction : public IEntityAction
{
public:
	MoveAction(const MathLib::Vector2F & targetPosition);

protected:
	virtual void onStart(ControllableGameObject * gameObject) override;
	virtual void onEnd(ControllableGameObject * gameObject) override;
	virtual void onCancel(ControllableGameObject * gameObject) override;
	virtual void onStateChange(ControllableGameObject * gameObject, EntityState oldState, EntityState newState) override;

private:
	MathLib::Vector2F targetPosition;
	bool rotated;

};

class CollectingAction : public IEntityAction
{
public:
	CollectingAction(double collecting_time, CollectableGameObject * collectable, float maximumDistance);

protected:
	virtual void onStart(ControllableGameObject * gameObject) override;
	virtual void onEnd(ControllableGameObject * gameObject) override;
	virtual void onCancel(ControllableGameObject * gameObject) override;
	virtual void onStateChange(ControllableGameObject * gameObject, EntityState oldState, EntityState newState) override;

private:
	float maximum_distance;	// distance to collect
	CollectableGameObject * collectable;
	double collecting_time;

};

class AttackAction : public IEntityAction
{
public:
	AttackAction(LiveGameObject * object, float maximum_distance);

protected:
	virtual void onStart(ControllableGameObject * gameObject) override;
	virtual void onEnd(ControllableGameObject * gameObject) override;
	virtual void onCancel(ControllableGameObject * gameObject) override;
	virtual void onStateChange(ControllableGameObject * gameObject, EntityState oldState, EntityState newState) override;

	LiveGameObject * target;
	float maximum_distance;

};
