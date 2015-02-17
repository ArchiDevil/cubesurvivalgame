#pragma once

#include <GraphicsEngine/ShiftEngine.h>

#include "Objects/GUIMisc.h"

namespace SimpleGUI
{
    class Base;
    class Button;
    class Image;
    class List;
    class TableRow;
    class Text;
    class Textbox;
    class ValueBox;
    class Window;

    class Skinner
    {
    public:
        Skinner();
        ~Skinner();

        void Initialize();

        template<typename T>
        void Draw(T * item);

        void DrawButton(Button * but);
        void DrawTextbox(Textbox * textbox, const std::string & string, bool Focused);
        void DrawImage(Image * image, const ShiftEngine::Sprite & spr);
        void DrawList(List * list);
        void DrawListRow(TableRow * row);
        void DrawLabel(Text * label);
        void DrawValueBox(ValueBox * valueBox);
        void DrawWindow(Window * window);

    protected:
        void SetControlParameters(ShiftEngine::Sprite * spr, const Point & pos, const Point & size);
        void LoadResources();

        ShiftEngine::Sprite * ButtonCache;
        ShiftEngine::Sprite * TextboxCache;
        ShiftEngine::Sprite * ListCache;

        // just a simple 1x1 box
        ShiftEngine::Sprite * boxCache;
    };
}
