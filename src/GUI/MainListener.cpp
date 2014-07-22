#include "MainListener.h"

namespace SimpleGUI
{
	MainListener::MainListener( Canvas * _pCanvas, cInputEngine * _eng, std::vector<wchar_t> & _charQueue )
		: Engine(_eng), charQueue(_charQueue), Listener(_pCanvas)
	{}

	void MainListener::Update()
	{
		auto mouseInfo = Engine->GetMouseInfo();

		Point oldPos = Point(mouseInfo.clientX - mouseInfo.deltaX, mouseInfo.clientY - mouseInfo.deltaY);
		Point newPos = Point(mouseInfo.clientX, mouseInfo.clientY);

		if(Engine->IsMouseMoved())
			Handler.ProcessMouseMoving(oldPos, newPos);

		if(Engine->IsMouseUp(LButton))
			Handler.ProcessMouseUp(MouseEventInfo(LButton, newPos.x, newPos.y));

		if(Engine->IsMouseUp(RButton))
			Handler.ProcessMouseUp(MouseEventInfo(RButton, newPos.x, newPos.y));

		if(Engine->IsMouseDown(LButton))
			Handler.ProcessMouseDown(MouseEventInfo(LButton, newPos.x, newPos.y));

		if(Engine->IsMouseDown(RButton))
			Handler.ProcessMouseDown(MouseEventInfo(RButton, newPos.x, newPos.y));

		if(Engine->IsKeyUp(DIK_ESCAPE))
		{
			Handler.ProcessSpecialKeyUp(DIK_ESCAPE);
		}

		if(!charQueue.empty())
		{
			Handler.ProcessKeyChar(charQueue[0]);
			charQueue.clear();
		}
		//some actions
	}
}
