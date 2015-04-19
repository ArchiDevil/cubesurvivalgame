#pragma once

#include <algorithm>

#include "appclass.h"
#include "cAppStateBase.h"

#include <MyGUI.h>
#include <MyGUI_DirectX11Platform.h>

class Application;

class MenuState : 
    public appState,
    observer<InputEvent>
{
public:
    MenuState(Application * _pApp, MyGUI::Gui * pGui, MyGUI::DirectX11Platform * pPlatform);
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

    void Create(MyGUI::Widget* _sender);
    void LoadFile(MyGUI::Widget* _sender);
    void RemoveFile(MyGUI::Widget* _sender);

    virtual bool handleEvent(const InputEvent & event);

    Application *               pApp = nullptr;

    MyGUI::Gui *                pGui = nullptr;
    MyGUI::DirectX11Platform *  pPlatform = nullptr;

    // ui for open dialog
    MyGUI::Window *             pWindow = nullptr;
};
