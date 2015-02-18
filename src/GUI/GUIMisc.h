#pragma once

#include <Utilities/ut.h>
#include <MathLib/math.h>

#include <memory>

using MathLib::Vector2I;
using MathLib::Vector2F;

namespace SimpleGUI
{
    class Base;

    class Canvas;
    class Skinner;
    class MainListener;

    // hide them ALL!
    extern Base *           HoveredControl;
    extern Base *           FocusedControl;
    extern Canvas *         ActiveCanvas;
    extern Skinner *        ActiveSkinner;
    extern MainListener *   ActiveListener;

    typedef Vector2I        Point;
    typedef Vector2F        FloatSize;
    typedef std::string     AnsiString;
    typedef std::wstring    UnicodeString;  //cannot have 32-bit on Windows system

    FloatSize CalculateScaling(int needX, int needY, int actualX, int actualY);
    void SetCanvas(Canvas * pCanvas);
    void SetSkinner(Skinner * pSkinner);
    void SetMainListener(MainListener * pListener);
    void DrawUI();

    struct Area
    {
        Area(int Top, int Bottom, int Left, int Right)
            : Top(Top)
            , Bottom(Bottom)
            , Left(Left)
            , Right(Right)
        {
        }

        int Top, Bottom, Left, Right;
    };

    enum EventType
    {
        m_MOUSEBEGIN,
        EventClick,
        EventMove,
        m_MOUSEEND,
        EventChar,
    };

    struct Event
    {
        Event(EventType Type, MouseInfo MI, unsigned char Key)
            : Type(Type)
            , MI(MI)
            , Key(Key)
        {
        }

        EventType Type;
        MouseInfo MI;
        unsigned char Key;
    };
}
