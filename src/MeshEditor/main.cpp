#include <ctime>

#include "appclass.h" //Win32 application class

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, PSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
    srand((unsigned)time(nullptr));

    IniWorker i;
    if (!i.Initialize("settings.ini"))
        LOG_ERROR("Unable to open settings");

    Application app(hInstance, i.GetInteger("Width"), i.GetInteger("Height"), L"LIME");

    return app.Run();
}
