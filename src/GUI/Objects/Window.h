#pragma once

#include "Base.h"
#include <GraphicsEngine\Sprite.h>

#include <vector>

namespace SimpleGUI
{
    class Window : public Base
    {
    public:
        Window(Base * parent/*const Point & _position, const Point & _size*/);
        virtual ~Window();
        virtual void Draw(SimpleGUI::Skinner * skin) override;
        virtual bool OnMouseDown(MouseKeys mb, int x, int y) override;
        virtual bool OnMouseUp(MouseKeys mb, int x, int y) override;
        virtual bool OnMouseMove() override;

    private:
        std::vector<Base *> Objects;
        bool dragged;
    };
}
