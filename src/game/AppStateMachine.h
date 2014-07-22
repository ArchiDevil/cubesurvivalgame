#pragma once

#include <stack>

#include "appState.h"

class AppStateMachine
{
public:
	AppStateMachine();
	~AppStateMachine();

	void PushState( appState * state );
	appState * GetTopState() const;
	void Suspend();
	void Resume();
	bool Frame(double dt);

private:
	std::stack<appState *> statesStack; //stack of game states like MainMenu, GameState and more

};