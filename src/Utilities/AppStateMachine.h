#pragma once

#include <stack>

#include <Utilities/IAppState.h>

class AppStateMachine
{
public:
    void PushState(IAppState * state);
    IAppState * GetTopState() const;
    void Suspend();
    void Resume();
    bool Frame(double dt);

private:
    std::stack<IAppState *> statesStack; //stack of game states like MainMenu, GameState and more

};
