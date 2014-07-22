#include "cApplication.h"

cApplication::cApplication( HINSTANCE hInstance, int Width, int Height, LPCWSTR AppName )
{
	System.InitializeWindow(hInstance, Width, Height, AppName);
}

cApplication::~cApplication()
{
}

int cApplication::Run()
{
	Initialize();

	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	while(msg.message != WM_QUIT)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			ProcessMessage(msg);

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			if(!Frame())
				break;
		}
	}

	Shutdown();

	return 0;
}

bool cApplication::CreateDefaultWindow( HINSTANCE hInstance, int Width, int Height, LPCWSTR AppName )
{
	if(System.InitializeWindow(hInstance, Width, Height, AppName))
		return true;
	else
		return false;
}

HWND cApplication::GetHWND() const
{
	return System.GetHWND();
}

HINSTANCE cApplication::GetHINSTANCE() const
{
	return System.GetHINSTANCE();
}
