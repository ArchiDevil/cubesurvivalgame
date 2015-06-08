#include "AppStateMachine.h"

void AppStateMachine::PushState(IAppState * state)
{
    if (!statesStack.empty())
    {
        statesStack.top()->onSuspend();
    }
    statesStack.push(state);
    statesStack.top()->initState();
}

IAppState * AppStateMachine::GetTopState() const
{
    return !statesStack.empty() ? nullptr : statesStack.top();
}

void AppStateMachine::Suspend()
{
    if (!statesStack.empty())
        statesStack.top()->onSuspend();
}

void AppStateMachine::Resume()
{
    if (!statesStack.empty())
        statesStack.top()->onResume();
}

bool AppStateMachine::Frame(double dt)
{
    if (!statesStack.empty())
    {
        if (statesStack.top()->isDead())
        {
            IAppState * state = statesStack.top();
            statesStack.pop();
            state->onKill();
            delete state;

            if (!statesStack.empty())
                statesStack.top()->onResume();
            return true;        //skip frame
        }

        if (!statesStack.top()->update(dt))     //use current state
            return false;
        if (!statesStack.top()->render(dt))
            return false;
        return true;
    }

    return false;
}
