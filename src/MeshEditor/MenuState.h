#pragma once

#include <algorithm>

#include "appclass.h"
#include "cAppStateBase.h"

#include "../GUI/GUI.h"

class Application;

class MenuState : public appState
{
public:
	MenuState(SimpleGUI::Canvas * _pCanvas, SimpleGUI::Skinner * _pSkinner, Application * _pApp);
	~MenuState();

	virtual bool initState();
	virtual bool update( double dt );
	virtual bool render( double dt );
	virtual void onKill();
	virtual void onSuspend();
	virtual void onResume();

private:
	bool ProcessInput();
	void CreateGUI();

	Application * pApp;

	SimpleGUI::Canvas * pCanvas;
	SimpleGUI::Skinner * pSkinner;

};