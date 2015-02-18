#include "Window.h"

#include "Button.h"

SimpleGUI::Window::Window(Base * parent)
    : Window{ parent, { 0, 0 }, { 100, 100 } }
{
}

SimpleGUI::Window::Window(Base * parent, const Point & _position, const Point & _size)
    : Base(parent)
    , dragged(false)
{
    Base::SetPosition(_position);
    Base::SetSize(_size.x, _size.y);

    Button * closeButton = new Button(this);
    closeButton->SetSize(16, 16);
    closeButton->SetText("X");
    closeButton->SetPosition(2, 2);
    closeButton->SetClickHandler([this](MouseKeys, int, int){ Hide(); });
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
    if (mb != LButton)
        return false;

    dragged = true;
    oldInner = { x, y };
    return true;
}

bool SimpleGUI::Window::OnMouseUp(MouseKeys mb, int /*x*/, int /*y*/)
{
    if (mb != LButton)
        return false;

    dragged = false;
    oldInner = { -1, -1 };
    return true;
}

bool SimpleGUI::Window::OnMouseMove(Point /*oldPos*/, Point newPos)
{
    if (!dragged)
        return false;

    // may work wrong if we have parent with different from 0;0 position
    // for now assume that we have only canvases as parents
    Point pos = GetPosition();
    Point delta = newPos - oldInner;
    oldInner = newPos;
    SetPosition(pos + delta);

    return true;
}

void SimpleGUI::Window::SetDraggable(bool draggable)
{
    this->draggable = draggable;
}

bool SimpleGUI::Window::IsDraggable() const
{
    return draggable;
}
