#include "appclass.h"

//TEMPORARY

int PerfCounter = 0;
int minFPS = 100000;
int maxFPS = 0;
double AllTime = 0.0f;

//END OF TEMPORARY

Log PerformanceLog("Performance.log");

Application::Application( int Width, int Height, LPCWSTR AppName )
	: cApplication(Width, Height, AppName)
	, MainCanvas(nullptr)
	, GUIListener(nullptr)
{
	//	MainCanvas			= new Canvas;
}

Application::~Application()
{
	//	delete MainCanvas;
}

bool Application::Initialize()
{
	////////////////////////////
	/// TECH STRUCTURES INIT ///
	////////////////////////////

	//�������������� ��� ��������� .ini ������
	SettingsLoader.Initialize("settings.ini");

	//��������� ��������� ��� ������������ ������ �� ����� ��������
	ShiftEngine::GraphicEngineSettings settings;
	settings.screenHeight			= SettingsLoader.GetInteger("Height");
	settings.screenWidth			= SettingsLoader.GetInteger("Width");
	settings.multisampleQuality		= SettingsLoader.GetInteger("MultisampleQuality");
	settings.windowed				= SettingsLoader.GetBoolean("Windowed");
	settings.screenRate				= SettingsLoader.GetInteger("ScreenRate");
	settings.zNear					= SettingsLoader.GetFloat("zNear");
	settings.zFar					= SettingsLoader.GetFloat("zFar");
	settings.anisotropyLevel		= SettingsLoader.GetInteger("AnisotropyLevel");

	//��������� ������ � �����
	ShiftEngine::PathSettings path;
	path.MeshPath					= SettingsLoader.GetWString("MeshPath");
	path.TexturePath				= SettingsLoader.GetWString("TexturePath");
	path.ShaderPath					= SettingsLoader.GetWString("ShaderPath");
	path.FontsPath					= SettingsLoader.GetWString("FontsPath");
	path.MaterialsPath				= SettingsLoader.GetWString("MaterialsPath");

	//�������������� ����������� ������
	if(!ShiftEngine::InitEngine(ShiftEngine::AT_DX10, settings, path, GetHWND()))
		MainLog.FatalError("Unable to inititalize graphics engine");
	else
		MainLog.Message("Graphics engine has been initialized");

	if(!cInputEngine::GetInstance().Initialize(GetHWND(), GetHINSTANCE()))
		MainLog.FatalError("Unable to create DIDevice");
	else
		MainLog.Message("Input system has been initialized");

	//	MainCanvas->SetSize(settings.screenWidth, settings.screenHeight);

	////////////////////////////
	/// GAME STRUCTURES INIT ///
	////////////////////////////

	gameState * state = new gameState(&SettingsLoader);
	statesStack.push(state);

	state->initState();

	this->SaveTechInfo();
	MainLog.Message("Successfully initialized, starting main loop");
	gameTimer.Start();

	return true;
}

void Application::Shutdown()
{
	ShiftEngine::ShutdownEngine();

	PerformanceLog.Message("Min FPS = "			+ std::to_string(minFPS)							+ " FPS");
	PerformanceLog.Message("Average FPS = "		+ std::to_string((int)(PerfCounter / AllTime))		+ " FPS");
	PerformanceLog.Message("Max FPS = "			+ std::to_string(maxFPS)							+ " FPS");
	PerformanceLog.Message("Frames elapsed = "	+ std::to_string(PerfCounter)						+ " frames");
	PerformanceLog.Message("All time = "		+ std::to_string((int)AllTime)						+ " seconds");
	MainLog.Message("Working done");
}

bool Application::Frame()
{
	static double elapsedTime = 0.0f;

	elapsedTime = gameTimer.GetDeltaTime();
	PerfCounter++;
	AllTime += elapsedTime;
	gameTimer.Tick();

	int curFPS = ShiftEngine::GetRenderer()->GetFPS();

	if(curFPS > maxFPS)
		maxFPS = curFPS;
	if(curFPS < minFPS)
		minFPS = curFPS;

	if(!statesStack.empty())
	{
		if(statesStack.top()->isDead())
		{
			auto * state = statesStack.top();
			statesStack.pop();
			state->onKill();
			delete state;

			if(!statesStack.empty())
				statesStack.top()->onResume();
			return true;		//skip frame
		}

		if(!statesStack.top()->update(elapsedTime))		//use current state
			return false;
		if(!statesStack.top()->render(elapsedTime))
			return false;
		return true;
	}
	else
	{
		return false;
	}
}

void Application::PushState( appState * state )
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

void Application::ProcessMessage( MSG msg )
{
	static AppState prevState = AS_Running;
	AppState currState = System.GetState();

	if(prevState != currState)
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
	//�������� ���������� � ���������� � ��� �������
	PerformanceLog.Message("Processor name: " + utils::WStrToStr(RegistryWorker.GetString(HKEY_LOCAL_MACHINE_R, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", L"ProcessorNameString")));
	PerformanceLog.Message("Frequency = " + std::to_string(RegistryWorker.GetInteger(HKEY_LOCAL_MACHINE_R, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", L"~MHz")) + " MHz");

	//�������� ���������� ����������� ������ � ������� � � �������������
	//�����, ���������� ��� ��� � ����
	MEMORYSTATUSEX memoryStatus;
	memoryStatus.dwLength = sizeof(memoryStatus);
	GlobalMemoryStatusEx(&memoryStatus);
	PerformanceLog.Message("Total physical memory: " + std::to_string((int64_t)memoryStatus.ullTotalPhys / 1024 / 1024) + " MBs");
	PerformanceLog.Message("Using: " + std::to_string((int)memoryStatus.dwMemoryLoad) + "%");
	PerformanceLog.Message("Videocard description: " + utils::WStrToStr(ShiftEngine::GetContextManager()->GetVideocardDesc()));
}