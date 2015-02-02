#include "appclass.h"

using namespace SimpleGUI;

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
    //�������������� ��� ��������� .ini ������
    SettingsLoader.Initialize("settings.ini");

    //��������� ��������� ��� ������������ ������ �� ����� ��������
    ShiftEngine::GraphicEngineSettings settings;
    settings.screenHeight = SettingsLoader.GetInteger("Height");
    settings.screenWidth = SettingsLoader.GetInteger("Width");
    settings.multisampleQuality = SettingsLoader.GetInteger("MultisampleQuality");
    settings.windowed = SettingsLoader.GetBoolean("Windowed");
    settings.screenRate = SettingsLoader.GetInteger("ScreenRate");
    settings.anisotropyLevel = SettingsLoader.GetInteger("AnisotropyLevel");
    settings.zNear = SettingsLoader.GetFloat("zNear");
    settings.zFar = SettingsLoader.GetFloat("zFar");

    //��������� ������ � �����
    ShiftEngine::PathSettings path;
    path.MeshPath = SettingsLoader.GetWString("MeshPath");
    path.TexturePath = SettingsLoader.GetWString("TexturePath");
    path.ShaderPath = SettingsLoader.GetWString("ShaderPath");
    path.FontsPath = SettingsLoader.GetWString("FontsPath");
    path.MaterialsPath = SettingsLoader.GetWString("MaterialsPath");

    //�������������� ����������� ������
    ShiftEngine::InitEngine(ShiftEngine::AT_DX10, settings, path, GetHWND());
    LOG_INFO("DirectX has been initialized");

    ShiftEngine::GetSceneGraph()->AddCameraSceneNode();

    if (!cInputEngine::GetInstance().Initialize(GetHWND(), GetHINSTANCE()))
        LOG_INFO("Unable to create DIDevice");
    else
        LOG_INFO("Input system has been initialized");

    GUIListener = new SimpleGUI::MainListener(nullptr, &(cInputEngine::GetInstance()), charQueue);
    SimpleGUI::ActiveListener = GUIListener;

    MenuState * state = new MenuState(this);
    statesStack.push(state);

    state->initState();

    mainTimer.Start();
    ShiftEngine::GetContextManager()->SetBlendingState(ShiftEngine::BS_AlphaEnabled);

    return true;
}

void Application::Shutdown()
{
    ShiftEngine::GetContextManager()->Shutdown();
}

bool Application::Frame()
{
    if (System.GetState() == AS_Inactive)
        return true;

    static double elapsedTime = 0.0f;

    elapsedTime = mainTimer.GetDeltaTime();
    mainTimer.Tick();

    GUIListener->Update();

    //anyone, explain me that code =)
    if (statesStack.size() != 0)
    {
        if (statesStack.top()->isDead())
        {
            auto * state = statesStack.top();
            statesStack.pop();
            state->onKill();
            delete state;

            if (statesStack.size() != 0)
                statesStack.top()->onResume();
            return true;		//skip frame
        }

        if (!statesStack.top()->update(elapsedTime))		//use current state
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
        charQueue.push_back((wchar_t)msg.wParam);
    }
    default:
        break;
    }
}
