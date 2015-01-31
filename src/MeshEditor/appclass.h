#pragma once

#include <stack>

#include <Utilities/System/Application.h>
#include <GraphicsEngine/ShiftEngine.h>

#include <Utilities/timer.h>
#include <Utilities/cRegWorker.h>
#include <Utilities/InputEngine.h>
#include <Utilities/iniloader.h>

#include "MenuState.h"
#include "WorkState.h"

class Application : public cApplication
{
public:
    Application(HINSTANCE hInstance, int Width, int Height, LPCWSTR AppName);
    ~Application();

    bool Initialize();
    void Shutdown();
    void PushState(appState * state);

    bool Frame();
    void ProcessMessage(MSG msg);

    void Stop();
    void Activate();

private:
    //////////////////////
    // OTHER STRUCTURES //
    //////////////////////
    cTimer							mainTimer;
    IniWorker						SettingsLoader;

    std::stack<appState *>			statesStack;	//stack of game states like MainMenu, GameState and more
    std::vector<wchar_t>			charQueue;

    SimpleGUI::MainListener *		GUIListener;
    SimpleGUI::Canvas *				MainCanvas;
    SimpleGUI::Skinner *			Skin;

};
