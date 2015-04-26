#pragma once

#include <utilities/ut.h>
#include <utilities/iniloader.h>
#include <Utilities/appState.h>

class mainMenuState : public appState
{
public:
    mainMenuState();
    ~mainMenuState();

    bool initState() override;
    bool update(double dt) override;
    bool render(double dt) override;
    void onKill() override;
    void onSuspend() override;
    void onResume() override;
};
