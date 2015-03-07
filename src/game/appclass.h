#pragma once

#include <stack>
#include <sstream>

#include <GraphicsEngine/ShiftEngine.h>

#include <SimplePhysicsEngine/PhysicsEngine.h>

#include <Utilities/System/Application.h>
#include <Utilities/timer.h>
#include <Utilities/cRegWorker.h>
#include <Utilities/InputEngine.h>
#include <Utilities/iniloader.h>

#include <GUI/GUI.h>

#include "appState.h"
#include "gameStates/gameState.h"
#include "gameStates/mainMenuState.h"
#include "game.h"

class Application : public cApplication
{
public:
    Application(int Width, int Height, LPCWSTR AppName);
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

    std::unique_ptr<SimpleGUI::MainListener> GUIListener;
    std::vector<wchar_t>                     charQueue;

    std::stack<appState *>	                 statesStack;		//stack of game states like MainMenu, GameState and more
    cTimer					                 gameTimer;			//frame timer
    IniWorker				                 SettingsLoader;	//settings loader
    cRegWorker				                 RegistryWorker;	//registry handler

};
