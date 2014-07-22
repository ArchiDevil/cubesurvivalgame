#include <ctime>

#include "appclass.h" //Win32 application class

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd)
{
	srand(time(nullptr));

	IniWorker i;
	if(!i.Initialize("settings.ini"))
	{
		MainLog.Error(true, L"Unable to open settings");
	}

	Application app(hInstance, i.GetInteger("Width"), i.GetInteger("Height"), L"LIME");

	return app.Run();
}