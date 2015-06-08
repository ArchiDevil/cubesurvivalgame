#pragma once

#include <stack>
#include <sstream>

#include <Utilities/IAppState.h>
#include <Utilities/AppStateMachine.h>
#include <Utilities/System/Application.h>
#include <Utilities/timer.h>
#include <Utilities/cRegWorker.h>
#include <Utilities/InputEngine.h>
#include <Utilities/iniloader.h>

#include <SimplePhysicsEngine/PhysicsEngine.h>

#include <GraphicsEngine/ShiftEngine.h>

#include <MyGUI.h>
#include <MyGUI_DirectX11Platform.h>

#include "gameStates/GameState.h"
#include "gameStates/MainMenuState.h"
#include "game.h"

class LostIslandApplication : public Application
{
public:
    LostIslandApplication(int Width, int Height, LPCWSTR AppName);
    ~LostIslandApplication();

    bool Initialize() override;
    bool Frame() override;
    void Shutdown() override;
    void Stop();
    void Activate();
    void ProcessMessage(MSG msg) override;
    void PushState(IAppState * state);
    IAppState * GetTopState() const;

private:
    void SaveTechInfo();

    AppStateMachine                             stateMachine;
    cTimer                                      gameTimer;         //frame timer
    IniWorker                                   settingsLoader;    //settings loader
    cRegWorker                                  registryWorker;    //registry handler

    MyGUI::Gui *                                guiModule = nullptr;
    MyGUI::DirectX11Platform *                  guiPlatform = nullptr;

};
