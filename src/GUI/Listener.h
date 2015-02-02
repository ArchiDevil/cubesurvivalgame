#pragma once

#include "Objects\Canvas.h"
#include "InputHandler.h"

namespace SimpleGUI
{
    class Listener
    {
    public:
        Listener(Canvas * _pCanvas);

        InputHandler * GetInputHandler();
        virtual void Update() = 0;

    protected:
        InputHandler Handler;
    };
}
