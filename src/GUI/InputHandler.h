#pragma once

#include "Objects/Canvas.h"

namespace SimpleGUI
{
	struct MouseEventInfo
	{
		MouseEventInfo(int _button, int _x, int _y)
			: button(_button)
            , x(_x)
            , y(_y) 
        {
        }

		int button, x, y;
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
		void UpdateHoveredControl(int mouseX, int mouseY);

		Canvas * pCanvas;
	};
}
