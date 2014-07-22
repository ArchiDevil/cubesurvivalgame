#include "AppStateMachine.h"

AppStateMachine::AppStateMachine()
{
}

AppStateMachine::~AppStateMachine()
{
}

void AppStateMachine::PushState( appState * state )
{
	if(!statesStack.empty())
	{
		statesStack.top()->onSuspend();
	}
	statesStack.push(state);
	statesStack.top()->initState();
}

appState * AppStateMachine::GetTopState() const
{
	return !statesStack.empty() ? nullptr : statesStack.top();
}

void AppStateMachine::Suspend()
{
	if(!statesStack.empty())
		statesStack.top()->onSuspend();
}

void AppStateMachine::Resume()
{
	if(!statesStack.empty())
		statesStack.top()->onResume();
}

bool AppStateMachine::Frame( double dt )
{
	if(!statesStack.empty())
	{
		if(statesStack.top()->isDead())
		{
			appState * state = statesStack.top();
			statesStack.pop();
			state->onKill();
			delete state;

			if(!statesStack.empty())
				statesStack.top()->onResume();
			return true;		//skip frame
		}

		if(!statesStack.top()->update(dt))		//use current state
			return false;
		if(!statesStack.top()->render(dt))
			return false;
		return true;
	}
	else
	{
		return false;
	}
	return true;
}