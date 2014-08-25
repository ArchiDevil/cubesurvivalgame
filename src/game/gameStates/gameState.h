#pragma once

#include "../appState.h"
#include "../game.h"
#include "../Console.h"

#include <Utilities/ut.h>
#include <Utilities/iniloader.h>
#include <Utilities/InputEngine.h>

using MathLib::Vector3D;
using MathLib::Vector2D;

class gameState : public appState
{
public:
	gameState(IniWorker * iw);
	~gameState();

	virtual bool initState();
	virtual bool update( double dt );
	virtual bool render( double dt );
	virtual void onKill();
	virtual void onSuspend();
	virtual void onResume();

private:
	void ProcessInput(double dt);
	IniWorker * pIniLoader;
	//Console console;

	//bool isConsoleState;

};
