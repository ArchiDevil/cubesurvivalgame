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
    virtual ~IEntityAction() {}

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
    bool m_dead = false;
    bool m_started = false;
    bool m_cancelled = false;
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

class MoveAction final : public IEntityAction
{
public:
    MoveAction(const MathLib::Vector2F & targetPosition);

protected:
    virtual void onStart(ControllableGameObject * gameObject) override;
    virtual void onEnd(ControllableGameObject * gameObject) override;
    virtual void onCancel(ControllableGameObject * gameObject) override;
    virtual void onStateChange(ControllableGameObject * gameObject, EntityState oldState, EntityState newState) override;

private:
    MathLib::Vector2F targetPosition = {};
    bool rotated = false;

};

class CollectingAction final : public IEntityAction
{
public:
    CollectingAction(double collecting_time, CollectableGameObject * collectable, float maximumDistance);

protected:
    virtual void onStart(ControllableGameObject * gameObject) override;
    virtual void onEnd(ControllableGameObject * gameObject) override;
    virtual void onCancel(ControllableGameObject * gameObject) override;
    virtual void onStateChange(ControllableGameObject * gameObject, EntityState oldState, EntityState newState) override;

private:
    float maximum_distance = 0.0f; // distance to collect
    CollectableGameObject * collectable = nullptr;
    double collecting_time = 0.0;

};

class AttackAction final : public IEntityAction
{
public:
    AttackAction(LiveGameObject * object, float maximum_distance);

protected:
    virtual void onStart(ControllableGameObject * gameObject) override;
    virtual void onEnd(ControllableGameObject * gameObject) override;
    virtual void onCancel(ControllableGameObject * gameObject) override;
    virtual void onStateChange(ControllableGameObject * gameObject, EntityState oldState, EntityState newState) override;

    LiveGameObject * target = nullptr;
    float maximum_distance = 0.0f;

};
