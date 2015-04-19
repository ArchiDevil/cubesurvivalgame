#include "appclass.h"

Application::Application(HINSTANCE /*hInstance*/, int Width, int Height, LPCWSTR AppName)
    : cApplication(Width, Height, AppName)
{
    utils::filesystem::CreateDir(L"saves");
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
    settings.screenHeight = SettingsLoader.GetInteger("Height");
    settings.screenWidth = SettingsLoader.GetInteger("Width");
    settings.multisampleQuality = SettingsLoader.GetInteger("MultisampleQuality");
    settings.windowed = SettingsLoader.GetBoolean("Windowed");
    settings.screenRate = SettingsLoader.GetInteger("ScreenRate");
    settings.anisotropyLevel = SettingsLoader.GetInteger("AnisotropyLevel");
    settings.zNear = SettingsLoader.GetFloat("zNear");
    settings.zFar = SettingsLoader.GetFloat("zFar");

    //загружаем данные о путях
    ShiftEngine::PathSettings path;
    path.MeshPath = SettingsLoader.GetWString("MeshPath");
    path.TexturePath = SettingsLoader.GetWString("TexturePath");
    path.ShaderPath = SettingsLoader.GetWString("ShaderPath");
    path.FontsPath = SettingsLoader.GetWString("FontsPath");
    path.MaterialsPath = SettingsLoader.GetWString("MaterialsPath");

    //инициализируем графический движок
    ShiftEngine::InitEngine(ShiftEngine::AT_DX11, settings, path, GetHWND());
    LOG_INFO("DirectX has been initialized");

    ShiftEngine::GetSceneGraph()->AddCameraSceneNode();

    if (!InputEngine::GetInstance().Initialize(GetHWND(), GetHINSTANCE()))
        LOG_INFO("Unable to create DIDevice");
    else
        LOG_INFO("Input system has been initialized");

    mainTimer.Start();
    ShiftEngine::GetContextManager()->SetBlendingState(ShiftEngine::BlendingState::AlphaEnabled);

    pPlatform = new MyGUI::DirectX11Platform();
    std::string logFile = "MyGUI.log";
#ifdef NDEBUG
    logFile = "";
#endif // NDEBUG
    pPlatform->initialise(((ShiftEngine::D3D11ContextManager*)ShiftEngine::GetContextManager())->GetDevicePtr(), logFile);
    pPlatform->getDataManagerPtr()->addResourceLocation("resources/ui/MyGUI_Media", false);

    pGui = new MyGUI::Gui();
    pGui->initialise();

    pPlatform->getRenderManagerPtr()->setViewSize(settings.screenWidth, settings.screenHeight);

    MenuState * state = new MenuState(this, pGui, pPlatform);
    statesStack.push(state);
    state->initState();

    return true;
}

void Application::Shutdown()
{
    if (pGui)
        pGui->shutdown();
    delete pGui;

    if (pPlatform)
        pPlatform->shutdown();
    delete pPlatform;
}

bool Application::Frame()
{
    if (System.GetState() == AS_Inactive)
        return true;

    static double elapsedTime = 0.0f;

    elapsedTime = mainTimer.GetDeltaTime();
    mainTimer.Tick();

    if (statesStack.size() != 0)
    {
        if (statesStack.top()->isDead())
        {
            appState * state = statesStack.top();
            statesStack.pop();
            state->onKill();
            delete state;

            if (statesStack.size() != 0)
                statesStack.top()->onResume();
            return true; //skip frame
        }

        if (!statesStack.top()->update(elapsedTime)) //use current state
            return false;
        if (!statesStack.top()->render(elapsedTime))
            return false;
        return true;
    }

    return false;
}

void Application::PushState(appState * state)
{
    statesStack.top()->onSuspend();
    statesStack.push(state);
    statesStack.top()->initState();
}

void Application::Stop()
{
    mainTimer.Stop();
    statesStack.top()->onSuspend();
}

void Application::Activate()
{
    mainTimer.Start();
    statesStack.top()->onResume();
}

void Application::ProcessMessage(MSG msg)
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

    switch (msg.message)
    {
    case WM_CHAR:
    {
        InputEngine::GetInstance().handleEvent(SystemKeyMessage(msg.wParam));
        charQueue.push_back((wchar_t)msg.wParam);
    }
    default:
        break;
    }
}
