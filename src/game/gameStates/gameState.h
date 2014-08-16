#pragma once

#include "../appState.h"
#include "../game.h"
#include "../../utilities/ut.h"
#include "../../utilities/iniloader.h"
#include "../../utilities/InputEngine.h"

using MathLib::Vector3D;
using MathLib::Vector2D;

class gameState : public appState
{
public:
	gameState(cGame * _game, IniWorker * iw);
	~gameState();

	virtual bool initState();
	virtual bool update( double dt );
	virtual bool render( double dt );
	virtual void onKill();
	virtual void onSuspend();
	virtual void onResume();

private:
	void ProcessInput(double dt);

	bool inventoryFlag;

	cGame * pGame;
	IniWorker * pIniLoader;

};
