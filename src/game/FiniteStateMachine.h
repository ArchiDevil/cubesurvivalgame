#pragma once

#include <memory>
#include <map>

enum class EntityState;
class IEntityState;

// may be templated by entity state
class FiniteStateMachine
{
public:
    FiniteStateMachine();
    virtual ~FiniteStateMachine();

    void AddTransition(EntityState from, EntityState to);
    bool DispatchState(std::unique_ptr<IEntityState> state);
    EntityState GetCurrentStateType() const;

protected:
    bool CanDispatchState(EntityState state) const;
    virtual void OnStateChange(EntityState from, EntityState to);
    IEntityState * GetCurrentState();

private:
    std::multimap<EntityState, EntityState> transitions;
    std::unique_ptr<IEntityState> currentState;

};
