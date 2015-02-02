#include "Listener.h"

namespace SimpleGUI
{
    Listener::Listener(Canvas * _pCanvas)
        : Handler(_pCanvas)
    {
    }

    InputHandler * Listener::GetInputHandler()
    {
        return &Handler;
    }

}
