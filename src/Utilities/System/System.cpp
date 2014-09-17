#include "System.h"

cSystem::cSystem()
	: State(AS_Running)
	, WindowHandler(0)
	, WindowInstance(0)
{
	System = this;
}

cSystem::~cSystem() 
{
}

HWND cSystem::GetHWND() const
{
	return WindowHandler;
}

HINSTANCE cSystem::GetHINSTANCE() const
{
	return WindowInstance;
}

LRESULT CALLBACK cSystem::MessageHandler( HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch(msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_ACTIVATE:
		if(wParam == WA_ACTIVE)
			State = AS_Running;
		if(wParam == WA_CLICKACTIVE)
			State = AS_Running;
		if(wParam == WA_INACTIVE)
			State = AS_Inactive;
		break;
	case WM_CHAR:
		//long key = wParam;
		//switch (key)
		//{
		//case 0x08: 
		//	notifyAll(SystemKeyMessage(SK_BACKSPACE));
		//	break;
		//case 0x0A:
		//case 0x0D:
		//	notifyAll(SystemKeyMessage(SK_ENTER));
		//	break;
		//case 0x1B:
		//	notifyAll(SystemKeyMessage(SK_ESC));
		//	break;
		//case 0x09:
		//	notifyAll(SystemKeyMessage(SK_TAB));
		//	break;
		//default:
		//	notifyAll(SystemKeyMessage(SK_CHAR, key));
		//	break;
		//}
	default:
		break;
	}
	return ::DefWindowProc(windowHandle, msg, wParam, lParam); //это если ничего из вышеперечисленного не подошло
}

bool cSystem::InitializeWindow( int Width, int Height, LPCWSTR AppName )
{
	this->WindowInstance = GetModuleHandle(0);

	WNDCLASS wc;
	wc.cbClsExtra = 0;														//не требуется
	wc.cbWndExtra = 0;														//не требуется
	wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));	//закрашиваем фон белой кистью
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);								//курсор
	wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);								//значок
	wc.hInstance = this->WindowInstance;									//поле дескриптора, передаем в функцию
	wc.lpfnWndProc = WndProc;												//процедура обработки
	wc.lpszClassName = AppName;												//имя класса, нужно системе
	wc.lpszMenuName = 0;													//менюшка, нам не требуется
	wc.style = CS_HREDRAW | CS_VREDRAW;										//стили окна - перерисовываем при изменении размера по горизонтали и вертикали

	if(!::RegisterClass(&wc))
		return false;

	int originPosX = 0;
	int originPosY = 0;

	DWORD windowStyle = WS_OVERLAPPEDWINDOW &~(WS_MAXIMIZEBOX) &~ (WS_THICKFRAME); //non-resizeable

	RECT r = {originPosX, originPosY, originPosX + Width, originPosY + Height};
	AdjustWindowRect(&r, windowStyle, false);

	int actualWidth = r.right - r.left;
	int actualHeight = r.bottom - r.top;

	this->WindowHandler = ::CreateWindow(
		AppName,				//имя класса окна которое мы выше написали) в общем как-то так, я уже полусплю
		AppName,				//заголовок окна
		windowStyle,			//стиль создаваемого окна
		originPosX,				//координата по X
		originPosY,				//координата по Y
		actualWidth,			//ширина
		actualHeight,			//высота
		0,						//дескриптор родителя, родителя нет, следовательно 0
		0,						//дексриптор меню, его тоже нет
		this->WindowInstance,	//переданный сюда инстанс
		0);						//указатель на определяемые пользователем данные

	if(this->WindowHandler == 0)
		return false;

	::ShowWindow(this->WindowHandler, SW_SHOW);	//покажем созданное окно
	::UpdateWindow(this->WindowHandler);		//и обновим

	::ShowCursor(NULL);

	return true;
}

AppState cSystem::GetState() const
{
	return State;
}

LRESULT CALLBACK WndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return System->MessageHandler(windowHandle, msg, wParam, lParam);
}
