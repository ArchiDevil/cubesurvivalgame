#include "FiniteStateMachine.h"

#include "Entities/EntityState.h"

#include <Utilities/logger.h>

#include <cassert>
#include <string>

FiniteStateMachine::FiniteStateMachine()
	: currentState(std::make_unique<WaitingState>())
{
}

FiniteStateMachine::~FiniteStateMachine()
{
}

void FiniteStateMachine::AddTransition(EntityState from, EntityState to)
{
	transitions.insert(std::pair<EntityState, EntityState>(from, to));
}

bool FiniteStateMachine::CanDispatchState(EntityState state) const
{
	auto iter_pair = transitions.equal_range(GetCurrentStateType());
	for (auto iter = iter_pair.first; iter != iter_pair.second; ++iter)
		if (iter->second == state)
			return true;

	return false;
}

bool FiniteStateMachine::DispatchState(std::unique_ptr<IEntityState> state)
{
	LOG_INFO("Entity: " + std::to_string((size_t)this) + " moved into " + std::to_string((int)state->GetType()));
	EntityState fromState = currentState->GetType();
	EntityState toState = state->GetType();
	if (currentState && CanDispatchState(state->GetType()))
	{
		currentState = std::move(state);
		OnStateChange(fromState, toState);
		return true;
	}
	else
	{
		LOG_INFO("Entity " + std::to_string((size_t)this) + " cannot move from " + std::to_string((int)fromState) + " to " + std::to_string((int)toState));
		return false;
	}
}

EntityState FiniteStateMachine::GetCurrentStateType() const
{
	return currentState->GetType();
}

IEntityState * FiniteStateMachine::GetCurrentState()
{
	return currentState.get();
}

void FiniteStateMachine::OnStateChange(EntityState from, EntityState to)
{
}
