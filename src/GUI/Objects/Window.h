#pragma once

#include "Base.h"

namespace SimpleGUI
{
    class Window : public Base
    {
    public:
        Window(Base * parent);
        Window(Base * parent, const Point & _position, const Point & _size);
        virtual ~Window();
        virtual void Draw(SimpleGUI::Skinner * skin) override;
        virtual bool OnMouseDown(MouseKeys mb, int x, int y) override;
        virtual bool OnMouseUp(MouseKeys mb, int x, int y) override;
        virtual bool OnMouseMove(Point oldPos, Point newPos) override;
        void SetDraggable(bool draggable);
        bool IsDraggable() const;

        virtual void SetSize(int x, int y) override;
        virtual void SetSize(const Point & size) override;

    private:
        void SetCloseButtonPosition();

        bool dragged = false;
        bool draggable = false;
        Point oldInner = {};
        Button * closeButton = nullptr;
    };
}
