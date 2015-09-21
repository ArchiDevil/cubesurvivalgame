#include "LostIslandApplication.h"

//TEMPORARY

int PerfCounter = 0;
int minFPS = 100000;
int maxFPS = 0;
double AllTime = 0.0f;

//END OF TEMPORARY

Log PerformanceLog("Performance.log");

LostIslandApplication::LostIslandApplication(int Width, int Height, LPCWSTR AppName)
    : Application(Width, Height, AppName)
{
}

LostIslandApplication::~LostIslandApplication()
{
}

bool LostIslandApplication::Initialize()
{
    ::ShowCursor(false);

    //инициализируем наш загрузчик .ini файлов
    settingsLoader.Initialize("settings.ini");

    //загружаем настройки для графического движка из файла настроек
    ShiftEngine::GraphicEngineSettings settings;
    settings.screenHeight       = settingsLoader.GetInteger("Height");
    settings.screenWidth        = settingsLoader.GetInteger("Width");
    settings.multisampleQuality = settingsLoader.GetInteger("MultisampleQuality");
    settings.windowed           = settingsLoader.GetBoolean("Windowed");
    settings.screenRate         = settingsLoader.GetInteger("ScreenRate");
    settings.zNear              = settingsLoader.GetFloat("zNear");
    settings.zFar               = settingsLoader.GetFloat("zFar");
    settings.anisotropyLevel    = settingsLoader.GetInteger("AnisotropyLevel");

    //загружаем данные о путях
    ShiftEngine::PathSettings path;
    path.MeshPath      = settingsLoader.GetWString("MeshPath");
    path.TexturePath   = settingsLoader.GetWString("TexturePath");
    path.ShaderPath    = settingsLoader.GetWString("ShaderPath");
    path.FontsPath     = settingsLoader.GetWString("FontsPath");
    path.MaterialsPath = settingsLoader.GetWString("MaterialsPath");

    //инициализируем графический движок
    if (!ShiftEngine::InitEngine(settings, path, GetHWND()))
        LOG_FATAL_ERROR("Unable to inititalize graphics engine");
    else
        LOG_INFO("Graphics engine has been initialized");

    if (!InputEngine::GetInstance().Initialize(GetHWND(), GetHINSTANCE()))
        LOG_FATAL_ERROR("Unable to create DIDevice");
    else
        LOG_INFO("Input system has been initialized");

    InputEngine::GetInstance().subscribe(&System);

    guiPlatform = new MyGUI::DirectX11Platform();
    std::string logFile = "MyGUI.log";
#ifdef NDEBUG
    logFile = "";
#endif // NDEBUG
    // MyGUI::LogManager::getInstance().setSTDOutputEnabled(false);

    guiPlatform->initialise(((ShiftEngine::D3D11ContextManager*)ShiftEngine::GetContextManager())->GetDevicePtr(), logFile);
    guiPlatform->getDataManagerPtr()->addResourceLocation("resources/ui/MyGUI_Media", false);
    guiPlatform->getDataManagerPtr()->addResourceLocation("resources/textures", false);

    guiModule = new MyGUI::Gui();
    guiModule->initialise();

    guiPlatform->getRenderManagerPtr()->setViewSize(settings.screenWidth, settings.screenHeight);


    ////////////////////////////
    /// GAME STRUCTURES INIT ///
    ////////////////////////////

    GameState * state = new GameState(&settingsLoader, guiModule, guiPlatform);
    PushState(state);

    this->SaveTechInfo();
    LOG_INFO("Successfully initialized, starting main loop");
    gameTimer.Start();

    return true;
}

void LostIslandApplication::Shutdown()
{
    ShiftEngine::ShutdownEngine();

    PerformanceLog.Message("Min FPS = " + std::to_string(minFPS) + " FPS");
    PerformanceLog.Message("Average FPS = " + std::to_string((int)(PerfCounter / AllTime)) + " FPS");
    PerformanceLog.Message("Max FPS = " + std::to_string(maxFPS) + " FPS");
    PerformanceLog.Message("Frames elapsed = " + std::to_string(PerfCounter) + " frames");
    PerformanceLog.Message("All time = " + std::to_string((int)AllTime) + " seconds");
    LOG_INFO("Working done");
}

bool LostIslandApplication::Frame()
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

    return stateMachine.Frame(elapsedTime);
}

void LostIslandApplication::PushState(IAppState * state)
{
    stateMachine.PushState(state);
}

void LostIslandApplication::Stop()
{
    gameTimer.Stop();
    stateMachine.Suspend();
}

void LostIslandApplication::Activate()
{
    gameTimer.Start();
    stateMachine.Resume();
}

void LostIslandApplication::ProcessMessage(MSG /*msg*/)
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

void LostIslandApplication::SaveTechInfo()
{
    //собираем информацию о процессоре и его частоте
    PerformanceLog.Message("Processor name: " + utils::Narrow(registryWorker.GetString(HKEY_LOCAL_MACHINE_R, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", L"ProcessorNameString")));
    PerformanceLog.Message("Frequency = " + std::to_string(registryWorker.GetInteger(HKEY_LOCAL_MACHINE_R, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", L"~MHz")) + " MHz");

    //получаем количество оперативной памяти в системе и её загруженность
    //после, записываем все это в файл
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);
    PerformanceLog.Message("Total physical memory: " + std::to_string((int64_t)memoryStatus.ullTotalPhys / 1024 / 1024) + " MBs");
    PerformanceLog.Message("Using: " + std::to_string((int)memoryStatus.dwMemoryLoad) + "%");
    PerformanceLog.Message("Videocard description: " + utils::Narrow(ShiftEngine::GetContextManager()->GetGPUDescription()));
}

IAppState * LostIslandApplication::GetTopState() const
{
    return stateMachine.GetTopState();
}
