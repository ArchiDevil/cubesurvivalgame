#include "gameState.h"

#include <GraphicsEngine/ShiftEngine.h>

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
	cGame * pGame = LostIsland::GetGamePtr();

	console.subscribe(&cInputEngine::GetInstance());

	::utils::filesystem::CreateDir(L"saves/worlds/");
	::utils::filesystem::CreateDir(L"saves/players/");
	::utils::filesystem::CreateDir(L"saves/worlds/tempWorld/");

	int ChunksPerSide = pIniLoader->GetInteger("ChunksPerSide");
	int CenterX = (int)pIniLoader->GetFloat("PlayerXPosition") / (int)pGame->World->GetDataStorage()->GetChunkWidth();
	int CenterY = (int)pIniLoader->GetFloat("PlayerYPosition") / (int)pGame->World->GetDataStorage()->GetChunkWidth();
	pGame->World->Initialize(ChunksPerSide, CenterX, CenterY, "tempWorld");
	LOG_INFO("World Manager has been initialized");

	SimplePhysicsEngine::GetInstance().Initialize(pGame->World->GetDataStorage());
	LOG_INFO("Physics initialized");

	pGame->ItemMgr.reset(new ItemManager());
	pGame->ItemMgr->Initialize(L"resources/gamedata/Items/");
	LOG_INFO("Items have been loaded");

	pGame->gameHud->Initialize();
	LOG_INFO("HUD has been created");

	pGame->Player = pGame->EntityMgr->CreatePlayer(Vector3F()).get();

	pGame->environmentMgr->Initialize(dayTimer(11, 00));
	pGame->EntityMgr->LoadEntities();

	pScene->AddCameraSceneNode();
	pScene->SetAmbientColor(Vector3F(0.1f, 0.1f, 0.15f));
	pScene->GetActiveCamera()->SetPosition(0.0f, 0.0f, 0.0f);
	pScene->GetActiveCamera()->RotateByQuaternion(MathLib::quaternionFromVecAngle(Vector3F(1.0f, 0.0f, 0.0f), degrad(-60.0f)));
	pSun = pScene->AddDirectionalLightNode(Vector3F());

	pGame->EntityMgr->CreateEntity(Vector3F(-10.0, 10.0, 100.0), "stone");
	pGame->EntityMgr->CreateEntity(Vector3F(10.0, 10.0, 100.0), "tree1");
	pGame->EntityMgr->CreateItemEntity(Vector3F(-7.0f, 0.0f, 120.0f), Vector3F(), pGame->ItemMgr->GetItemId("stone"), 2);

	LOG_INFO("End of game state initializing");

	return true;
}

bool gameState::update(double dt)
{
	ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
	cGame * pGame = LostIsland::GetGamePtr();

	Vector3F playerPosition = pGame->Player->GetPosition();
	Vector3F sunPosition = pGame->environmentMgr->GetSunPosition(playerPosition);
	pSun->SetDirection(-sunPosition);
	ProcessInput(dt);
	SimplePhysicsEngine::GetInstance().Update(dt);
	pGame->EntityMgr->Update(dt);
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
	const int infoSize = 6;
	std::ostringstream di[infoSize];

	di[0] << "FPS: " << pRenderer->GetFPS();
	di[1] << "Shader changes: " << pRenderer->GetShaderChanges();
	di[2] << "Matrix bindings: " << pRenderer->GetMatricesBindings();
	di[3] << "Uniform bindings: " << pRenderer->GetUniformsBindings();
	di[4] << "Texture bindings: " << pRenderer->GetTextureBindings();
	di[5] << "Draw calls: " << pRenderer->GetDrawCalls();
#else
	const int infoSize = 2;
	std::ostringstream di[infoSize];
	di[0] << "FPS: " << pRenderer->GetFPS();
	di[1] << "Time of day: " << pGame->environmentMgr->GetTime().getHours() << ":" 
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
}

void gameState::onSuspend()
{
}

void gameState::onResume()
{
}

void gameState::ProcessInput(double dt)
{
	cInputEngine * InputEngine = &cInputEngine::GetInstance();
	ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
	ShiftEngine::D3D10ContextManager * pCtxMgr = ShiftEngine::GetContextManager();
	cGame * pGame = LostIsland::GetGamePtr();

	static size_t mousePath = 0;

	InputEngine->GetKeys();
	auto mouseInfo = InputEngine->GetMouseInfo();

	if (InputEngine->IsKeyUp(DIK_GRAVE))
		console.SetVisibility(!console.IsVisible());

	if (console.IsVisible())
		return;

	if (InputEngine->IsKeyDown(DIK_ESCAPE))
		this->kill();

	auto camPos = pScene->GetActiveCamera()->GetPosition();
	Vector3F newCamPos(camPos.x, camPos.y, camPos.z);
	pScene->GetActiveCamera()->SetPosition(newCamPos.x, newCamPos.y, newCamPos.z);

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
		theta -= (float)mouseInfo.deltaY * (float)dt * 10.0f;
		phi += (float)mouseInfo.deltaX * (float)dt * 10.0f;
		if (theta <= -35.0f)
			theta = -35.0f;
		if (theta >= -5.0f)
			theta = -5.0f;
	}

	// do the raycasting
	mat4f projMatrix = pScene->GetActiveCamera()->GetProjectionMatrix();
	mat4f viewMatrix = pScene->GetActiveCamera()->GetViewMatrix();
	Vector3F resultNear = MathLib::getUnprojectedVector(Vector3F((float)mouseInfo.clientX, (float)mouseInfo.clientY, 0.0f), projMatrix, viewMatrix);
	Vector3F resultFar = MathLib::getUnprojectedVector(Vector3F((float)mouseInfo.clientX, (float)mouseInfo.clientY, 1.0f), projMatrix, viewMatrix);
	Ray unprojectedRay = Ray(resultNear, MathLib::Normalize(resultFar - resultNear));

	pGame->EntityMgr->HighlightEntity(unprojectedRay);

	if (InputEngine->IsMouseUp(RButton))
	{
		// check if we are targeting to entity
		auto retVal = pGame->EntityMgr->GetNearestEntity(unprojectedRay);
		if (retVal)
		{
			InteractableGameObject* entity = (InteractableGameObject*)retVal.get();
			if (!entity) throw;
			auto interaction = entity->GetInteraction();
			pGame->Player->Interact(entity, interaction);
		}
		else
		{
			if (mousePath <= 15)
			{
				Vector3F column;
				if (pGame->World->SelectColumnByRay(unprojectedRay, column))
				{
					pGame->Player->CancelCurrentCommand();
					pGame->Player->Go(Vector2F(column.x, column.y));
				}
			}
		}
		mousePath = 0;
	}

	if (InputEngine->IsMouseDown(RButton))
		mousePath += abs(mouseInfo.deltaX) + abs(mouseInfo.deltaY);

	r -= (float)mouseInfo.deltaZ * (float)dt;
	if (r > 60.0f)
		r = 60.0f;
	if (r < 15.0f)
		r = 15.0f;

	if (InputEngine->IsKeyUp(DIK_1)) pGame->gameHud->SelectSlot(0);
	if (InputEngine->IsKeyUp(DIK_2)) pGame->gameHud->SelectSlot(1);
	if (InputEngine->IsKeyUp(DIK_3)) pGame->gameHud->SelectSlot(2);
	if (InputEngine->IsKeyUp(DIK_4)) pGame->gameHud->SelectSlot(3);
	if (InputEngine->IsKeyUp(DIK_5)) pGame->gameHud->SelectSlot(4);
	if (InputEngine->IsKeyUp(DIK_6)) pGame->gameHud->SelectSlot(5);
	if (InputEngine->IsKeyUp(DIK_7)) pGame->gameHud->SelectSlot(6);
	if (InputEngine->IsKeyUp(DIK_8)) pGame->gameHud->SelectSlot(7);
	if (InputEngine->IsKeyUp(DIK_9)) pGame->gameHud->SelectSlot(8);
	if (InputEngine->IsKeyUp(DIK_0)) pGame->gameHud->SelectSlot(9);
}
