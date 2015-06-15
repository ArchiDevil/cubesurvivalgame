#pragma once

#include <stack>

#include <System/Win32/Application.h>
#include <System/InputEngine.h>
#include <System/cRegWorker.h>

#include <GraphicsEngine/ShiftEngine.h>

#include <Utilities/timer.h>
#include <Utilities/iniloader.h>
#include <Utilities/AppStateMachine.h>

#include <MyGUI.h>
#include <MyGUI_DirectX11Platform.h>

#include "MenuState.h"
#include "WorkState.h"

class MeshEditorApplication : public Application
{
public:
    MeshEditorApplication(HINSTANCE hInstance, int Width, int Height, LPCWSTR AppName);
    ~MeshEditorApplication();

    bool Initialize();
    void Shutdown();
    void PushState(IAppState * state);

    bool Frame();
    void ProcessMessage(MSG msg);

    void Stop();
    void Activate();

private:
    cTimer                      mainTimer;
    IniWorker                   settingsLoader;

    AppStateMachine             stateMachine;
    std::vector<wchar_t>        charQueue;

    MyGUI::Gui *                pGui = nullptr;
    MyGUI::DirectX11Platform *  pPlatform = nullptr;

};
