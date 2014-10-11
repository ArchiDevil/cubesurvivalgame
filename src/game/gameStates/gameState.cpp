#include "gameState.h"

#include <thread>

#include <GraphicsEngine/Renderer.h>
#include <GraphicsEngine/ShaderManager.h>

#include "../player/cInventory.h"

std::thread worldThread;
volatile bool exitFlag = false;

//TEMPORARY
const double AccelerationMultiplier = 10.0;
//END OF TEMPORARY

gameState::gameState(IniWorker * iw )
	: pIniLoader(iw), isConsoleState(false), console(800, 600)
{
}

gameState::~gameState()
{
}

bool gameState::initState()
{
	LostIsland::CreateGame();

	ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
	ShiftEngine::D3D10ContextManager * pCtxMgr = ShiftEngine::GetContextManager();
	cGame * pGame = LostIsland::GetGamePtr();

	console.subscribe(&cInputEngine::GetInstance());

	cSimplePhysicsEngine::GetInstance().Initialize(pGame->World->GetDataStorage());
	MainLog.Message("Physics initialized");

	::utils::filesystem::CreateDir(L"saves/worlds/");
	::utils::filesystem::CreateDir(L"saves/players/");
	::utils::filesystem::CreateDir(L"saves/worlds/tempWorld/");

	int ChunksPerSide = pIniLoader->GetInteger("ChunksPerSide");
	int CenterX = (int)pIniLoader->GetFloat("PlayerXPosition") / (int)pGame->World->GetDataStorage()->GetChunkWidth();
	int CenterY = (int)pIniLoader->GetFloat("PlayerYPosition") / (int)pGame->World->GetDataStorage()->GetChunkWidth();
	pGame->World->Initialize(ChunksPerSide, CenterX, CenterY, pGame->environmentMgr, "tempWorld");
	MainLog.Message("World Manager has been initialized");

	pGame->ItemMgr = new ItemManager(pGame->Player, pGame->World, pGame->World->GetTypesStorage());
	pGame->ItemMgr->Initialize(L"resources/gamedata/Items/");
	MainLog.Message("Items have been loaded");

	pGame->Player->Initialize(pGame->World->GetDataStorage(), pGame->ItemMgr);
	pGame->Player->SetPosition(0.0f, 0.0f, 100.0f);
	MainLog.Message("Player has been initialized");

	pGame->gameHud->Initialize(pCtxMgr->GetParameters().screenWidth, pCtxMgr->GetParameters().screenHeight);
	MainLog.Message("HUD has been created");

	pGame->Player->GetInventoryPtr()->GetHandPtr()->itemId = pGame->ItemMgr->GetItemId("raw_fish");
	pGame->Player->GetInventoryPtr()->GetHandPtr()->count = 25;

	worldThread = std::thread([&]
	{
		while(!exitFlag) 
			LostIsland::GetGamePtr()->World->ProcessLoading();
		Sleep(0);
	});

	pGame->environmentMgr->Initialize(dayTimer(11, 00));

	auto settings = pCtxMgr->GetParameters();
	pScene->AddCameraSceneNode();
	pScene->SetAmbientColor(Vector3F(0.4f, 0.4f, 0.4f));
	pScene->GetActiveCamera()->SetPosition(0.0f, 0.0f, 0.0f);
	pScene->GetActiveCamera()->RotateByQuaternion(MathLib::quaternionFromVecAngle(Vector3F(1.0f, 0.0f, 0.0f), degrad(-60.0f)));

	MainLog.Message("End of game state initializing");

	return true;
}

bool gameState::update( double dt )
{
	static double accumulatedTime = 0.0;
	accumulatedTime += dt;

	ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
	cGame * pGame = LostIsland::GetGamePtr();

	ProcessInput(dt);
	Vector3F sunPos = pGame->environmentMgr->GetSkyPtr()->GetSunPos();
	cSimplePhysicsEngine::GetInstance().Update(dt);
	pGame->EntityMgr->Update(dt, sunPos);
	pGame->environmentMgr->Update(dt * 0.0, pGame->Player->GetPosition());

	Vector3D vectemp = pGame->Player->GetPosition();
	auto camPos = pScene->GetActiveCamera()->GetPosition();
	camPos.z = vectemp.z + 10.0f;
	pScene->GetActiveCamera()->SetPosition(camPos);

	if ((int)accumulatedTime % 10 == 0)
	{
		pGame->Player->SetHunger(pGame->Player->GetHunger() - 1);
		accumulatedTime++;
	}

	::SetCursorPos(600, 400);

	return true;
}

bool gameState::render( double dt )
{
	ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
	ShiftEngine::D3D10ContextManager * pCtxMgr = ShiftEngine::GetContextManager();
	ShiftEngine::Renderer * pRenderer = ShiftEngine::GetRenderer();
	cGame * pGame = LostIsland::GetGamePtr();

	auto camPos = pScene->GetActiveCamera()->GetPosition();
	Vector3D playerPos = pGame->Player->GetPosition();
	auto freePos = pGame->Player->GetSelectedBlockPtr()->GetPositions().free;

#if defined (DEBUG) || (_DEBUG)
	const int infoSize = 11;
	std::ostringstream di[infoSize];

	di[0] << "Health: " << pGame->Player->GetHealth();
	di[1] << "Warmth: " << pGame->Player->GetTemperature();
	di[2] << "Hunger: " << pGame->Player->GetHunger();
	di[3] << "FPS: " << pRenderer->GetFPS();
	di[4] << "Camera pos: " << camPos.x << " " << camPos.y << " " << camPos.z;
	di[5] << "Shader changes: " << pRenderer->GetShaderChanges();
	di[6] << "Matrix bindings: " << pRenderer->GetMatricesBindings();
	di[7] << "Uniform bindings: " << pRenderer->GetUniformsBindings();
	di[8] << "Texture bindings: " << pRenderer->GetTextureBindings();
	di[9] << "Draw calls: " << pRenderer->GetDrawCalls();
	di[10] << "Selected block pos: " << freePos.x << " " << freePos.y << " " << freePos.z;
#else
	const int infoSize = 5;
	std::ostringstream di[infoSize];
	di[0] << "Health: " << pGame->Player->GetHealth();
	di[1] << "Warmth: " << pGame->Player->GetTemperature();
	di[2] << "Hunger: " << pGame->Player->GetHunger();
	di[3] << "FPS: " << pRenderer->GetFPS();
	di[4] << "Time of day: " << pGame->environmentMgr->GetTime().getHours() << ":" 
		<< pGame->environmentMgr->GetTime().getMinutes();
#endif

	////////////
	// RENDER //
	////////////

	ShiftEngine::FontManager * pFntMgr = pCtxMgr->GetFontManager();
	pFntMgr->SetFont(L"1");

	pCtxMgr->SetBlendingState(ShiftEngine::BS_AlphaEnabled);

	pCtxMgr->BeginScene(); //no more needed here, cause clear frame should be called from renderer

		pScene->DrawAll(dt);

		pCtxMgr->SetZState(false);
			pGame->gameHud->Draw();
		pCtxMgr->SetZState(true);

	for (int i = 0; i < infoSize ; i++)
		pFntMgr->DrawTextTL(di[i].str(), 0, 0 + i*32);

	if(isConsoleState)
		console.Draw();

	pCtxMgr->EndScene();

	return true; //return false if something wrong
}

void gameState::onKill()
{
	exitFlag = true;
	if(worldThread.joinable())
		worldThread.join();
}

void gameState::onSuspend()
{
	exitFlag = true;
	if(worldThread.joinable())
		worldThread.join();
}

void gameState::onResume()
{
	cGame * pGame = LostIsland::GetGamePtr();

	if(worldThread.joinable())
		worldThread.join();
	exitFlag = false;
	worldThread = std::thread([&]
	{
		while(!exitFlag) 
			LostIsland::GetGamePtr()->World->ProcessLoading();
		Sleep(0);
	});
}

void gameState::ProcessInput(double dt)
{
	cInputEngine * InputEngine = &cInputEngine::GetInstance();
	cSimplePhysicsEngine * PhysicsEngine = &cSimplePhysicsEngine::GetInstance();
	ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
	ShiftEngine::D3D10ContextManager * pCtxMgr = ShiftEngine::GetContextManager();
	cGame * pGame = LostIsland::GetGamePtr();

	InputEngine->GetKeys();

	if(InputEngine->IsKeyUp(DIK_GRAVE))
	{
		isConsoleState = !isConsoleState;
	}

	if(isConsoleState)
	{
		if(InputEngine->IsKeyUp(DIK_RETURN))
			console.HandleCommand();

		return;
	}

	D3DXVECTOR3 tempVec = pScene->GetActiveCamera()->GetUpVector();
	tempVec.z = 0.0f;
	Vector3F upCam = MathLib::Normalize(Vector3F(tempVec.x, tempVec.y, tempVec.z));
	tempVec = pScene->GetActiveCamera()->GetRightVector();
	Vector3D rightCam(tempVec.x, tempVec.y, 0);

	if (InputEngine->IsKeyDown(DIK_ESCAPE))
		this->kill();

	double cameraSpeed = 10.0;
	if (InputEngine->IsKeyDown(DIK_LSHIFT)) //HACK: temporary
	{
		cameraSpeed *= AccelerationMultiplier;
	}

	auto camPos = pScene->GetActiveCamera()->GetPosition();
	Vector3F newCamPos(camPos.x, camPos.y, camPos.z);

	if (InputEngine->IsKeyDown(DIK_W))
	{
		newCamPos += upCam * dt * cameraSpeed;
	}

	if (InputEngine->IsKeyDown(DIK_S))
	{
		newCamPos -= upCam * dt * cameraSpeed;
	}

	if (InputEngine->IsKeyDown(DIK_A))
	{
		newCamPos -= rightCam * dt * cameraSpeed;
	}

	if (InputEngine->IsKeyDown(DIK_D))
	{
		newCamPos += rightCam * dt * cameraSpeed;
	}

	pScene->GetActiveCamera()->SetPosition(newCamPos.x, newCamPos.y, newCamPos.z);

	if (InputEngine->IsKeyUp(DIK_F))
		PhysicsEngine->ChangePlayerFreeState();

	if (InputEngine->IsKeyUp(DIK_V))
	{
		static bool Wflag = false;
		Wflag = !Wflag;
		if (Wflag)
			pCtxMgr->SetRasterizerState(ShiftEngine::RS_Wireframe);
		else
			pCtxMgr->SetRasterizerState(ShiftEngine::RS_Normal);
	}
}
