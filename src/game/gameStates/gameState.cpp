#include "gameState.h"

#include <GraphicsEngine/ShiftEngine.h>
#include "../cInventory.h"

#include <thread>

std::thread worldThread;
volatile bool exitFlag = false;

//TEMPORARY
float phi = 0.0f;
float theta = -35.0f;
float r = 20.0f;
ShiftEngine::LightNode * pSun = nullptr;
//END OF TEMPORARY

gameState::gameState(IniWorker * iw)
	: pIniLoader(iw)
	, console(800, 600)
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
	pGame->Player = pGame->EntityMgr->CreatePlayer(Vector3F(0.0f, 0.0f, 0.0f)).get();

	console.subscribe(&cInputEngine::GetInstance());

	//cSimplePhysicsEngine::GetInstance().Initialize(pGame->World->GetDataStorage());
	//MainLog.Message("Physics initialized");

	::utils::filesystem::CreateDir(L"saves/worlds/");
	::utils::filesystem::CreateDir(L"saves/players/");
	::utils::filesystem::CreateDir(L"saves/worlds/tempWorld/");

	int ChunksPerSide = pIniLoader->GetInteger("ChunksPerSide");
	int CenterX = (int)pIniLoader->GetFloat("PlayerXPosition") / (int)pGame->World->GetDataStorage()->GetChunkWidth();
	int CenterY = (int)pIniLoader->GetFloat("PlayerYPosition") / (int)pGame->World->GetDataStorage()->GetChunkWidth();
	pGame->World->Initialize(ChunksPerSide, CenterX, CenterY, "tempWorld");
	MainLog.Message("World Manager has been initialized");

	pGame->ItemMgr = new ItemManager(pGame->Player, pGame->World, pGame->World->GetTypesStorage());
	pGame->ItemMgr->Initialize(L"resources/gamedata/Items/");
	MainLog.Message("Items have been loaded");

	pGame->EntityMgr->CreatePlayer(Vector3F(0.0f, 0.0f, 100.0f));
	pGame->Player->Initialize();
	MainLog.Message("Player has been initialized");

	pGame->gameHud->Initialize(pCtxMgr->GetParameters().screenWidth, pCtxMgr->GetParameters().screenHeight);
	MainLog.Message("HUD has been created");

	pGame->Player->GetInventoryPtr()->GetHandPtr()->itemId = pGame->ItemMgr->GetItemId("raw_fish");
	pGame->Player->GetInventoryPtr()->GetHandPtr()->count = 25;

	//worldThread = std::thread([&]
	//{
	//while(!exitFlag) 
	//	LostIsland::GetGamePtr()->World->ProcessLoading();
	//Sleep(0);
	//});

	pGame->environmentMgr->Initialize(dayTimer(11, 00));

	auto settings = pCtxMgr->GetParameters();
	pScene->AddCameraSceneNode();
	pScene->SetAmbientColor(Vector3F(0.4f, 0.4f, 0.4f));
	pScene->GetActiveCamera()->SetPosition(0.0f, 0.0f, 0.0f);
	pScene->GetActiveCamera()->RotateByQuaternion(MathLib::quaternionFromVecAngle(Vector3F(1.0f, 0.0f, 0.0f), degrad(-60.0f)));

	pSun = pScene->AddDirectionalLightNode(Vector3F());

	MainLog.Message("End of game state initializing");

	return true;
}

bool gameState::update(double dt)
{
	ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
	cGame * pGame = LostIsland::GetGamePtr();

	Vector3F playerPosition = pGame->Player->GetPosition();
	Vector3F sunPosition = pGame->environmentMgr->calculateSunPos(playerPosition);
	pSun->SetDirection(-sunPosition);
	ProcessInput(dt);
	cSimplePhysicsEngine::GetInstance().Update(dt);
	pGame->EntityMgr->Update(dt, sunPosition);
	pGame->environmentMgr->Update(dt * 0.0);

	pScene->GetActiveCamera()->SetSphericalCoords(D3DXVECTOR3(playerPosition.x, playerPosition.y, playerPosition.z), phi, theta, r);
	pGame->World->ProcessLoading();

	return true;
}

bool gameState::render(double dt)
{
	ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
	ShiftEngine::D3D10ContextManager * pCtxMgr = ShiftEngine::GetContextManager();
	ShiftEngine::Renderer * pRenderer = ShiftEngine::GetRenderer();
	cGame * pGame = LostIsland::GetGamePtr();

#if defined (DEBUG) || (_DEBUG)
	const int infoSize = 9;
	std::ostringstream di[infoSize];

	di[0] << "Health: " << pGame->Player->GetHealth();
	di[1] << "Warmth: " << pGame->Player->GetTemperature();
	di[2] << "Hunger: " << pGame->Player->GetHunger();
	di[3] << "FPS: " << pRenderer->GetFPS();
	di[4] << "Shader changes: " << pRenderer->GetShaderChanges();
	di[5] << "Matrix bindings: " << pRenderer->GetMatricesBindings();
	di[6] << "Uniform bindings: " << pRenderer->GetUniformsBindings();
	di[7] << "Texture bindings: " << pRenderer->GetTextureBindings();
	di[8] << "Draw calls: " << pRenderer->GetDrawCalls();
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

	for (int i = 0; i < infoSize; i++)
		pFntMgr->DrawTextTL(di[i].str(), 0, 0 + i * 32);

	console.Draw();

	pCtxMgr->EndScene();

	return true; //return false if something wrong
}

void gameState::onKill()
{
	exitFlag = true;
	if (worldThread.joinable())
		worldThread.join();
}

void gameState::onSuspend()
{
	exitFlag = true;
	if (worldThread.joinable())
		worldThread.join();
}

void gameState::onResume()
{
	cGame * pGame = LostIsland::GetGamePtr();

	if (worldThread.joinable())
		worldThread.join();
	exitFlag = false;
	worldThread = std::thread([&]
	{
		while (!exitFlag)
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

	static size_t mousePath = 0;

	InputEngine->GetKeys();
	auto mouseInfo = InputEngine->GetMouseInfo();

	if (InputEngine->IsKeyUp(DIK_GRAVE))
	{
		console.SetVisibility(!console.GetVisibility());
	}

	if (console.GetVisibility())
	{
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

	if (InputEngine->IsMouseMoved() && InputEngine->IsMouseDown(RButton))
	{
		theta -= mouseInfo.deltaY * dt * 10.0f;
		phi += mouseInfo.deltaX * dt * 10.0f;
		if (theta <= -35.0f)
			theta = -35.0f;
		if (theta >= -5.0f)
			theta = -5.0f;
	}

	// do the raycasting
	mat4f projMatrix = pScene->GetActiveCamera()->GetProjectionMatrix();
	mat4f viewMatrix = pScene->GetActiveCamera()->GetViewMatrix();
	Vector3F resultNear = MathLib::getUnprojectedVector(Vector3F(mouseInfo.clientX, mouseInfo.clientY, 0.0f), projMatrix, viewMatrix);
	Vector3F resultFar = MathLib::getUnprojectedVector(Vector3F(mouseInfo.clientX, mouseInfo.clientY, 1.0f), projMatrix, viewMatrix);
	Ray unprojectedRay = Ray(resultNear, MathLib::Normalize(resultFar - resultNear));

	if (InputEngine->IsMouseUp(LButton))
	{
		pGame->EntityMgr->SelectEntity(unprojectedRay);
	}

	if (InputEngine->IsMouseUp(RButton))
	{
		if (mousePath <= 15)
		{
			Vector3F column;
			bool result = pGame->World->SelectColumnByRay(unprojectedRay, column);
			if (result)
			{
				MainLog.Message("Player goes to: " + std::to_string(column.x) + " " + std::to_string(column.y));
				pGame->Player->Go(Vector2F(column.x, column.y));
			}
		}
		mousePath = 0;
	}

	if (InputEngine->IsMouseDown(RButton))
	{
		mousePath += abs(mouseInfo.deltaX);
		mousePath += abs(mouseInfo.deltaY);
	}

	r -= mouseInfo.deltaZ * dt;
	if (r > 60.0f)
		r = 60.0f;
	if (r < 15.0f)
		r = 15.0f;
}
