#include "InputHandler.h"

// to add ctrl or shift holding
// set holding from environment (listener or something)
// i've not added it because i don't want to do much work =\

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
	InputHandler::InputHandler( Canvas * Canv )
		: pCanvas(Canv)	
	{
		HoveredControl = nullptr;
		FocusedControl = nullptr;
	}

	InputHandler::~InputHandler() {}

	void InputHandler::UpdateHoveredControl( int mouseX, int mouseY )
	{
		auto HC = pCanvas->GetControlAt(mouseX, mouseY);

		if(HC == HoveredControl && HC)
		{
			HoveredControl->OnMouseMove();
		}
		else
		{
			if(HoveredControl)
				HoveredControl->OnMouseLeave();

			HoveredControl = HC;

			if(HoveredControl)
				HoveredControl->OnMouseEnter();
		}
	}

	void InputHandler::ProcessMouseMoving( Point oldPos, Point newPos )
	{
		//much more?
		UpdateHoveredControl(newPos.x, newPos.y);
	}

	void InputHandler::ProcessKeyChar( wchar_t character )
	{
		if(FocusedControl)
			FocusedControl->OnKeyChar(character);
	}

	void InputHandler::ProcessSpecialKeyUp( unsigned int key )
	{
		return;
	}

	void InputHandler::ProcessMouseUp( const MouseEventInfo & event )
	{
		if(HoveredControl)
		{
			if(HoveredControl->CanHaveFocus())
				FocusedControl = HoveredControl;

			HoveredControl->OnMouseUp(event.button, event.x, event.y);

			return;
		}
	}

	void InputHandler::ProcessMouseDown( const MouseEventInfo & event )
	{
		if(HoveredControl)
		{
			if(HoveredControl->CanHaveFocus())
				FocusedControl = HoveredControl;

			HoveredControl->OnMouseDown(event.button, event.x, event.y);

			return;
		}
	}

}