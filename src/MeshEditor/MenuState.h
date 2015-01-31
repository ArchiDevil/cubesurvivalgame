#pragma once

#include <algorithm>

#include "appclass.h"
#include "cAppStateBase.h"

#include <GUI/GUI.h>

class Application;

class MenuState : public appState
{
public:
    MenuState(SimpleGUI::Canvas * _pCanvas, SimpleGUI::Skinner * _pSkinner, Application * _pApp);
    ~MenuState();

    virtual bool initState() override;
    virtual bool update(double dt) override;
    virtual bool render(double dt) override;
    virtual void onKill() override;
    virtual void onSuspend() override;
    virtual void onResume() override;

private:
    bool ProcessInput();
    void CreateGUI();

    Application * pApp;

    SimpleGUI::Canvas * pCanvas;
    SimpleGUI::Skinner * pSkinner;

};
