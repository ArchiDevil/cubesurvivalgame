#pragma once

#include <GraphicsEngine/ShiftEngine.h>

#include "GUIMisc.h"

namespace SimpleGUI
{
    class Base;
    class Button;
    class Image;
    class List;
    class TableRow;
    class Text;
    class TextBox;
    class ProgressBar;
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
        void DrawTextbox(TextBox * textbox, bool Focused);
        void DrawImage(Image * image, const ShiftEngine::Sprite & spr);
        void DrawList(List * list);
        void DrawListRow(TableRow * row);
        void DrawLabel(Text * label);
        void DrawValueBox(ValueBox * valueBox);
        void DrawWindow(Window * window);
        void DrawProgressBar(ProgressBar* pBar);

    protected:
        void SetControlParameters(ShiftEngine::Sprite * spr, const Point & pos, const Point & size);
        void LoadResources();
        std::unique_ptr<ShiftEngine::Sprite> ButtonCache;
        std::unique_ptr<ShiftEngine::Sprite> TextboxCache;
        std::unique_ptr<ShiftEngine::Sprite> ListCache;

        // just a simple 1x1 box
        std::unique_ptr<ShiftEngine::Sprite> boxCache;
    };
}
