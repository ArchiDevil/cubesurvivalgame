#include "Window.h"

#include <Utilities/logger.hpp>

SimpleGUI::Window::Window(Base * parent/*const Point & _position, const Point & _size*/)
    : Base(parent)
    , dragged(false)
{
    //Base::SetPosition(_position);
    //Base::SetSize(_size.x, _size.y);
}

SimpleGUI::Window::~Window()
{
}

void SimpleGUI::Window::Draw(SimpleGUI::Skinner * skin)
{
    if (!skin)
        return;

    skin->DrawWindow(this);
}

bool SimpleGUI::Window::OnMouseDown(MouseKeys mb, int x, int y)
{
    if (mb != 0)
        return false;

    dragged = true;
    LOG_INFO("Coords: ", mb, x, y);

    return true;
}

bool SimpleGUI::Window::OnMouseUp(MouseKeys mb, int x, int y)
{
    if (mb != 0)
        return false;

    dragged = true;
    LOG_INFO("Coords: ", mb, x, y);

    return true;
}

bool SimpleGUI::Window::OnMouseMove()
{
    if (!dragged)
        return false;
}
