#pragma once

#include <utilities/ut.h>
#include <utilities/iniloader.h>
#include <Utilities/IAppState.h>

class mainMenuState : public IAppState
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
