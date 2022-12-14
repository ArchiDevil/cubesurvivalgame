#include "MeshEditorApplication.h"

MeshEditorApplication::MeshEditorApplication(HINSTANCE /*hInstance*/, int Width, int Height, LPCWSTR AppName)
    : Application(Width, Height, AppName)
{
    utils::filesystem::CreateDir(L"saves");
}

MeshEditorApplication::~MeshEditorApplication()
{
}

bool MeshEditorApplication::Initialize()
{
    ::ShowCursor(false);

    //?????????????? ??? ????????? .ini ??????
    settingsLoader.Initialize("settings.ini");

    //????????? ????????? ??? ???????????? ?????? ?? ????? ????????
    ShiftEngine::GraphicEngineSettings settings;
    settings.screenHeight = settingsLoader.GetInteger("Height");
    settings.screenWidth = settingsLoader.GetInteger("Width");
    settings.multisampleQuality = settingsLoader.GetInteger("MultisampleQuality");
    settings.windowed = settingsLoader.GetBoolean("Windowed");
    settings.screenRate = settingsLoader.GetInteger("ScreenRate");
    settings.anisotropyLevel = settingsLoader.GetInteger("AnisotropyLevel");
    settings.zNear = settingsLoader.GetFloat("zNear");
    settings.zFar = settingsLoader.GetFloat("zFar");

    //????????? ?????? ? ?????
    ShiftEngine::PathSettings path;
    path.MeshPath = settingsLoader.GetWString("MeshPath");
    path.TexturePath = settingsLoader.GetWString("TexturePath");
    path.ShaderPath = settingsLoader.GetWString("ShaderPath");
    path.FontsPath = settingsLoader.GetWString("FontsPath");
    path.MaterialsPath = settingsLoader.GetWString("MaterialsPath");

    //?????????????? ??????????? ??????
    ShiftEngine::InitEngine(settings, path, GetHWND());
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
    MyGUI::LogManager::getInstance().setSTDOutputEnabled(false);

    pPlatform->initialise(((ShiftEngine::D3D11ContextManager*)ShiftEngine::GetContextManager())->GetDevicePtr(), logFile);
    pPlatform->getDataManagerPtr()->addResourceLocation("resources/ui/MyGUI_Media", false);

    pGui = new MyGUI::Gui();
    pGui->initialise();

    pPlatform->getRenderManagerPtr()->setViewSize(settings.screenWidth, settings.screenHeight);

    MenuState * state = new MenuState(this, pGui, pPlatform);
    PushState(state);

    return true;
}

void MeshEditorApplication::Shutdown()
{
    if (pGui)
        pGui->shutdown();
    delete pGui;

    if (pPlatform)
        pPlatform->shutdown();
    delete pPlatform;
}

bool MeshEditorApplication::Frame()
{
    if (System.GetState() == AS_Inactive)
        return true;

    static double elapsedTime = 0.0f;

    elapsedTime = mainTimer.GetDeltaTime();
    mainTimer.Tick();

    return stateMachine.Frame(elapsedTime);
}

void MeshEditorApplication::PushState(IAppState * state)
{
    stateMachine.PushState(state);
}

void MeshEditorApplication::Stop()
{
    mainTimer.Stop();
    stateMachine.Suspend();
}

void MeshEditorApplication::Activate()
{
    mainTimer.Start();
    stateMachine.Resume();
}

void MeshEditorApplication::ProcessMessage(MSG msg)
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
