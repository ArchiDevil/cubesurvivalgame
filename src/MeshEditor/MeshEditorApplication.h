#pragma once

#include <stack>

#include <Utilities/System/Application.h>
#include <GraphicsEngine/ShiftEngine.h>

#include <Utilities/timer.h>
#include <Utilities/cRegWorker.h>
#include <Utilities/InputEngine.h>
#include <Utilities/iniloader.h>

#include <MyGUI.h>
#include <MyGUI_DirectX11Platform.h>

#include "MenuState.h"
#include "WorkState.h"

class MeshEditorApplication : public cApplication
{
public:
    MeshEditorApplication(HINSTANCE hInstance, int Width, int Height, LPCWSTR AppName);
    ~MeshEditorApplication();

    bool Initialize();
    void Shutdown();
    void PushState(appState * state);

    bool Frame();
    void ProcessMessage(MSG msg);

    void Stop();
    void Activate();

private:
    cTimer                      mainTimer;
    IniWorker                   settingsLoader;

    std::stack<appState *>      statesStack;    //stack of game states like MainMenu, GameState and more
    std::vector<wchar_t>        charQueue;

    MyGUI::Gui *                pGui = nullptr;
    MyGUI::DirectX11Platform *  pPlatform = nullptr;

};
