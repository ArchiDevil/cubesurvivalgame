#pragma once

///////////////////////////////
/// DEFAULT LIBRARY HEADERS ///
///////////////////////////////

#include <stack>
#include <sstream>

//////////////////
/// MY HEADERS ///
//////////////////

#include "../GraphicsEngine/ShiftEngine.h"

#include "appState.h"
#include "gameStates/gameState.h"
#include "gameStates/mainMenuState.h"

#include "../utilities/System/Application.h"

#include "../SimplePhysicsEngine/PhysicsEngine.h"

#include "../utilities/timer.h"
#include "../utilities/cRegWorker.h"
#include "../utilities/InputEngine.h"
#include "../utilities/iniloader.h"

#include "game.h"

#include "../GUI/GUI.h"

using namespace SimpleGUI;

class Application : public cApplication
{
public:
	Application( int Width, int Height, LPCWSTR AppName );
	~Application();

	bool Initialize() override;
	bool Frame() override;
	void Shutdown() override;
	void Stop();
	void Activate();
	void ProcessMessage(MSG msg) override;
	void PushState(appState * state);

private:
	void SaveTechInfo();

	//////////////////////////
	// TECHNICAL STRUCTURES //
	//////////////////////////
	Canvas *				MainCanvas;			//canvas
	MainListener *			GUIListener;		//gui listener

	//////////////////////
	// OTHER STRUCTURES //
	//////////////////////
	std::stack<appState *>	statesStack;		//stack of game states like MainMenu, GameState and more
	cTimer					gameTimer;			//frame timer
	IniWorker				SettingsLoader;		//settings loader
	cRegWorker				RegistryWorker;		//registry handler

};