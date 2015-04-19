#pragma once

#include "../appState.h"
#include "../game.h"
#include "../../utilities/ut.h"
#include "../../utilities/iniloader.h"

class mainMenuState : public appState
{
public:
    mainMenuState();
    ~mainMenuState();

    virtual bool initState();
    virtual bool update(double dt);
    virtual bool render(double dt);
    virtual void onKill();
    virtual void onSuspend();
    virtual void onResume();

private:
};
