#pragma once

#include "Objects/Canvas.h"

namespace SimpleGUI
{
    struct MouseEventInfo
    {
        MouseEventInfo(MouseKeys key, int x, int y)
            : key(key)
            , x(x)
            , y(y)
        {
        }

        MouseKeys key;
        int x, y;
    };

    class InputHandler
    {
    public:
        InputHandler(Canvas * Canv);
        ~InputHandler();

        void ProcessKeyChar(wchar_t character);
        void ProcessMouseDown(const MouseEventInfo & event);
        void ProcessMouseUp(const MouseEventInfo & event);
        void ProcessMouseMoving(Point oldPos, Point newPos);
        void ProcessSpecialKeyUp(unsigned int key);

        const Canvas * GetCanvas() const;
        void SetCanvas(Canvas * pCanvas);

    private:
        Canvas * pCanvas;
    };
}
