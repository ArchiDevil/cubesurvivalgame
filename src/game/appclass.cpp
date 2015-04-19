#include "appclass.h"

//TEMPORARY

int PerfCounter = 0;
int minFPS = 100000;
int maxFPS = 0;
double AllTime = 0.0f;

//END OF TEMPORARY

Log PerformanceLog("Performance.log");

Application::Application(int Width, int Height, LPCWSTR AppName)
    : cApplication(Width, Height, AppName)
    , GUIListener(nullptr)
{
}

Application::~Application()
{
}

bool Application::Initialize()
{
    //инициализируем наш загрузчик .ini файлов
    SettingsLoader.Initialize("settings.ini");

    //загружаем настройки для графического движка из файла настроек
    ShiftEngine::GraphicEngineSettings settings;
    settings.screenHeight       = SettingsLoader.GetInteger("Height");
    settings.screenWidth        = SettingsLoader.GetInteger("Width");
    settings.multisampleQuality = SettingsLoader.GetInteger("MultisampleQuality");
    settings.windowed           = SettingsLoader.GetBoolean("Windowed");
    settings.screenRate         = SettingsLoader.GetInteger("ScreenRate");
    settings.zNear              = SettingsLoader.GetFloat("zNear");
    settings.zFar               = SettingsLoader.GetFloat("zFar");
    settings.anisotropyLevel    = SettingsLoader.GetInteger("AnisotropyLevel");

    //загружаем данные о путях
    ShiftEngine::PathSettings path;
    path.MeshPath      = SettingsLoader.GetWString("MeshPath");
    path.TexturePath   = SettingsLoader.GetWString("TexturePath");
    path.ShaderPath    = SettingsLoader.GetWString("ShaderPath");
    path.FontsPath     = SettingsLoader.GetWString("FontsPath");
    path.MaterialsPath = SettingsLoader.GetWString("MaterialsPath");

    //инициализируем графический движок
    if (!ShiftEngine::InitEngine(ShiftEngine::AT_DX11, settings, path, GetHWND()))
        LOG_FATAL_ERROR("Unable to inititalize graphics engine");
    else
        LOG_INFO("Graphics engine has been initialized");

    if (!InputEngine::GetInstance().Initialize(GetHWND(), GetHINSTANCE()))
        LOG_FATAL_ERROR("Unable to create DIDevice");
    else
        LOG_INFO("Input system has been initialized");

    InputEngine::GetInstance().subscribe(&System);

    GUIListener.reset(new SimpleGUI::MainListener(nullptr, &(InputEngine::GetInstance()), charQueue));
    SimpleGUI::ActiveListener = GUIListener.get();

    ////////////////////////////
    /// GAME STRUCTURES INIT ///
    ////////////////////////////

    gameState * state = new gameState(&SettingsLoader);
    statesStack.push(state);

    state->initState();

    this->SaveTechInfo();
    LOG_INFO("Successfully initialized, starting main loop");
    gameTimer.Start();

    return true;
}

void Application::Shutdown()
{
    ShiftEngine::ShutdownEngine();

    PerformanceLog.Message("Min FPS = " + std::to_string(minFPS) + " FPS");
    PerformanceLog.Message("Average FPS = " + std::to_string((int)(PerfCounter / AllTime)) + " FPS");
    PerformanceLog.Message("Max FPS = " + std::to_string(maxFPS) + " FPS");
    PerformanceLog.Message("Frames elapsed = " + std::to_string(PerfCounter) + " frames");
    PerformanceLog.Message("All time = " + std::to_string((int)AllTime) + " seconds");
    LOG_INFO("Working done");
}

bool Application::Frame()
{
    static double elapsedTime = 0.0f;

    elapsedTime = gameTimer.GetDeltaTime();
    PerfCounter++;
    AllTime += elapsedTime;
    gameTimer.Tick();

    int curFPS = ShiftEngine::GetRenderer()->GetFPS();

    if (curFPS > maxFPS)
        maxFPS = curFPS;
    if (curFPS < minFPS)
        minFPS = curFPS;

    GUIListener->Update();

    if (!statesStack.empty())
    {
        if (statesStack.top()->isDead())
        {
            auto * state = statesStack.top();
            statesStack.pop();
            state->onKill();
            delete state;

            if (!statesStack.empty())
                statesStack.top()->onResume();
            return true;		//skip frame
        }

        if (!statesStack.top()->update(elapsedTime))		//use current state
            return false;
        if (!statesStack.top()->render(elapsedTime))
            return false;
        return true;
    }
    else
    {
        return false;
    }
}

void Application::PushState(appState * state)
{
    statesStack.top()->onSuspend();
    statesStack.push(state);
    statesStack.top()->initState();
}

void Application::Stop()
{
    gameTimer.Stop();
    statesStack.top()->onSuspend();
}

void Application::Activate()
{
    gameTimer.Start();
    statesStack.top()->onResume();
}

void Application::ProcessMessage(MSG /*msg*/)
{
    static AppState prevState = AS_Running;
    AppState currState = System.GetState();

    if (prevState != currState)
    {
        switch (currState)
        {
        case AS_Running:
            this->Activate();
            break;
        case AS_Inactive:
            this->Stop();
            break;
        }
        prevState = currState;
    }
}

void Application::SaveTechInfo()
{
    //собираем информацию о процессоре и его частоте
    PerformanceLog.Message("Processor name: " + utils::Narrow(RegistryWorker.GetString(HKEY_LOCAL_MACHINE_R, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", L"ProcessorNameString")));
    PerformanceLog.Message("Frequency = " + std::to_string(RegistryWorker.GetInteger(HKEY_LOCAL_MACHINE_R, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", L"~MHz")) + " MHz");

    //получаем количество оперативной памяти в системе и её загруженность
    //после, записываем все это в файл
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);
    PerformanceLog.Message("Total physical memory: " + std::to_string((int64_t)memoryStatus.ullTotalPhys / 1024 / 1024) + " MBs");
    PerformanceLog.Message("Using: " + std::to_string((int)memoryStatus.dwMemoryLoad) + "%");
    PerformanceLog.Message("Videocard description: " + utils::Narrow(ShiftEngine::GetContextManager()->GetGPUDescription()));
}
