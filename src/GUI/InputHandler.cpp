#include "InputHandler.h"

// to add ctrl or shift holding
// set holding from environment (listener or something)
// I've not added it because I don't want to do much work =\

// какие события нужно покрыть?
// мышка:
//	move			V
//	enter			V
//	leave			V
//	wheel			X
//	down			V
//	up				V
// клавиатура:
//  press			X
//  down			X
//  up				X
//  <some>+<some>	X
//  char			V

namespace SimpleGUI
{
    InputHandler::InputHandler(Canvas * Canv)
        : pCanvas(Canv)
    {
        HoveredControl = nullptr;
        FocusedControl = nullptr;
    }

    InputHandler::~InputHandler()
    {
        HoveredControl = nullptr;
        FocusedControl = nullptr;
    }

    void InputHandler::ProcessMouseMoving(Point oldPos, Point newPos)
    {
        if (!pCanvas)
            return;

        Base * hovered = pCanvas->GetControlAt(newPos.x, newPos.y);

        if (hovered == HoveredControl && hovered)
        {
            Point pos = HoveredControl->GetPosition();
            HoveredControl->OnMouseMove(oldPos - pos, newPos - pos);
        }
        else
        {
            if (HoveredControl)
                HoveredControl->OnMouseLeave();

            HoveredControl = hovered;

            if (HoveredControl)
                HoveredControl->OnMouseEnter();
        }
    }

    void InputHandler::ProcessKeyChar(wchar_t character)
    {
        if (FocusedControl)
            FocusedControl->OnKeyChar(character);
    }

    void InputHandler::ProcessSpecialKeyUp(unsigned int /*key*/)
    {
        return;
    }

    void InputHandler::ProcessMouseUp(const MouseEventInfo & event)
    {
        if (!HoveredControl)
            return;

        if (HoveredControl->CanHaveFocus())
            FocusedControl = HoveredControl;

        Point pos = HoveredControl->GetPosition();
        HoveredControl->OnMouseUp(event.key, event.x - pos.x, event.y - pos.y);
    }

    void InputHandler::ProcessMouseDown(const MouseEventInfo & event)
    {
        if (!HoveredControl)
            return;

        if (HoveredControl->CanHaveFocus())
            FocusedControl = HoveredControl;

        Point pos = HoveredControl->GetPosition();
        HoveredControl->OnMouseDown(event.key, event.x - pos.x, event.y - pos.y);
    }

    const Canvas * InputHandler::GetCanvas() const
    {
        return pCanvas;
    }

    void InputHandler::SetCanvas(Canvas * pCanvas)
    {
        this->pCanvas = pCanvas;
    }
}
