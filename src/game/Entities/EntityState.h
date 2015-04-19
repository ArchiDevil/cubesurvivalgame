#pragma once

#include <MathLib/math.h>

#include <map>

class GameObject;
class ControllableGameObject;
class LiveGameObject;

enum class EntityState
{
    Waiting,
    Rotating,
    Moving,
    Dying,
    Decay,
    Collecting,
    Attacking
};

class IEntityState
{
public:
    virtual ~IEntityState() {}

    bool Dead() const;
    virtual EntityState GetType() const = 0;
    virtual void Update(LiveGameObject * entity, double dt) = 0;

protected:
    void Die();
    bool dead = false;
};

class WaitingState : public IEntityState
{
public:
    WaitingState();
    virtual void Update(LiveGameObject * entity, double dt) override;
    virtual EntityState GetType() const override;
};

class RotatingState : public IEntityState
{
public:
    RotatingState(const MathLib::Vector2F & targetPosition);
    virtual void Update(LiveGameObject * entity, double dt) override;
    virtual EntityState GetType() const override;

private:
    const MathLib::Vector2F targetPosition = {};
    double rotatingTime = 0.0;
};

class MovingState : public IEntityState
{
public:
    MovingState(const MathLib::Vector2F & targetPosition);
    virtual void Update(LiveGameObject * entity, double dt) override;
    virtual EntityState GetType() const override;

private:
    MathLib::Vector2F targetPosition = {};

};

class DyingState : public IEntityState
{
public:
    DyingState(double dyingTime);
    virtual void Update(LiveGameObject * entity, double dt) override;
    virtual EntityState GetType() const override;

private:
    double accumulatedTime = 0.0;
    const double fullTime;
};

class DecayState : public IEntityState
{
public:
    DecayState(float decayTime);
    virtual void Update(LiveGameObject * entity, double dt) override;
    virtual EntityState GetType() const override;

private:
    float elapsedTime = 0.0f;
};

class CollectingState : public IEntityState
{
public:
    CollectingState(double collectingTime);
    virtual EntityState GetType() const override;
    virtual void Update(LiveGameObject * entity, double dt) override;

private:
    const double collectingTime;
    double accumulatedTime = 0.0;

};

class AttackingState : public IEntityState
{
public:
    AttackingState(LiveGameObject * target, double cycleTime);
    virtual EntityState GetType() const override;
    virtual void Update(LiveGameObject * entity, double dt) override;

private:
    LiveGameObject * target;
    const double cycleTime;
    double accumulatedTime = 0.0;

};
