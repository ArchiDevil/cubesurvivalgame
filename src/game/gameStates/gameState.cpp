#include "gameState.h"

#include <thread>

#include <GraphicsEngine/Renderer.h>
#include <GraphicsEngine/ShaderManager.h>

#include "../player/cInventory.h"

std::thread worldThread;
volatile bool exitFlag = false;

//TEMPORARY
const double AccelerationMultiplier = 30.0f;
//END OF TEMPORARY

gameState::gameState(IniWorker * iw )
	: pIniLoader(iw)/*, isConsoleState(false)*/
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

	pGame->EntityMgr->CreateEntity(Vector3F(13.0f, 0.0f, 90.0f), "campfire");
	pGame->EntityMgr->CreateEntity(Vector3F(15.0f, 0.0f, 90.0f), "fishnet");

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
	
	D3DXVECTOR3 vec = pScene->GetActiveCamera()->GetLookVector();
	Vector3D pos = Vector3D(vec.x, vec.y, vec.z);
	pGame->Player->FindSelectedBlock(pos);

	auto items = pGame->EntityMgr->FindItemsNearPlayer(pGame->Player->GetPosition());
	for (auto iter = items.begin(); iter != items.end(); iter++)
	{
		if(pGame->GameEventHandler->onPlayerPicksItem((*iter)->GetItemId()))
		{
			(*iter)->Delete();
			pGame->Player->GetInventoryPtr()->GetHandPtr()->itemId = (*iter)->GetItemId();
			pGame->Player->GetInventoryPtr()->GetHandPtr()->count++;
		}
	}

	Vector3D vectemp = pGame->Player->GetPosition();
	pScene->GetActiveCamera()->SetPosition((float)vectemp.x, (float)vectemp.y, (float)vectemp.z + pGame->Player->GetHeight());

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

	Vector3F sunPos = pGame->environmentMgr->GetSkyPtr()->GetSunPos();
	Vector3D playerPos = pGame->Player->GetPosition();
	auto freePos = pGame->Player->GetSelectedBlockPtr()->GetPositions().free;

#if defined (DEBUG) || (_DEBUG)

	const int infoSize = 11;
	std::ostringstream di[infoSize];

	di[0] << "FPS: " << pRenderer->GetFPS();
	di[1] << "Player pos: " << playerPos.x << " " << playerPos.y << " " << playerPos.z;
	di[2] << "Shader changes: " << pRenderer->GetShaderChanges();
	di[3] << "Matrix bindings: " << pRenderer->GetMatricesBindings();
	di[4] << "Uniform bindings: " << pRenderer->GetUniformsBindings();
	di[5] << "Texture bindings: " << pRenderer->GetTextureBindings();
	di[6] << "Draw calls: " << pRenderer->GetDrawCalls();
	di[7] << "Selected block pos: " << freePos.x << " " << freePos.y << " " << freePos.z;
	di[8] << "Health: " << pGame->Player->GetHealth();
	di[9] << "Warmth: " << pGame->Player->GetTemperature();
	di[10] << "Hunger: " << pGame->Player->GetHunger();

#endif

#if defined (NDEBUG) || (_NDEBUG)

	const int infoSize = 5;
	std::ostringstream di[infoSize];
	di[0] << "FPS: " << pRenderer->GetFPS();
	di[1] << "Time of day: " << pGame->environmentMgr->GetTime().getHours() << ":" 
		<< pGame->environmentMgr->GetTime().getMinutes();
	di[2] << "Health: " << pGame->Player->GetHealth();
	di[3] << "Warmth: " << pGame->Player->GetTemperature();
	di[4] << "Hunger: " << pGame->Player->GetHunger();

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

	//if(isConsoleState)
	//	console.Draw();

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

	//if(InputEngine->IsKeyUp(DIK_GRAVE))
	//{
	//	console.ProcessInputKey('a');
	//	isConsoleState = !isConsoleState;
	//}

	//if(isConsoleState)
	//{
	//	if(InputEngine->IsKeyUp(DIK_RETURN))
	//		console.HandleCommand();

	//	return;
	//}

	D3DXVECTOR3 vec1 = pScene->GetActiveCamera()->GetLookVector();
	Vector3D vecNew = Vector3D(vec1.x, vec1.y, vec1.z);
	if (!PhysicsEngine->IsPlayerFree())
		vecNew.z = 0.0f;
	vecNew = MathLib::Normalize(vecNew);
	vec1 = pScene->GetActiveCamera()->GetRightVector();
	Vector3D vecRight = Vector3D(vec1.x, vec1.y, 0);

	if (InputEngine->IsKeyDown(DIK_ESCAPE))
		this->kill();

	if (InputEngine->IsKeyDown(DIK_LSHIFT)) //HACK: temporary
	{
		vecNew *= AccelerationMultiplier;
		vecRight *= AccelerationMultiplier;
	}

	bool moveFlag = false;

	if (InputEngine->IsKeyDown(DIK_W))
	{
		pGame->Player->SetVelocities(*pGame->Player->GetVelocitiesPtr() + vecNew * pGame->Player->GetSpeed());
		moveFlag = true;
	}

	if (InputEngine->IsKeyDown(DIK_S))
	{
		pGame->Player->SetVelocities(*pGame->Player->GetVelocitiesPtr() - vecNew * pGame->Player->GetSpeed());
		moveFlag = true;
	}

	if (InputEngine->IsKeyDown(DIK_A))
	{
		pGame->Player->SetVelocities(*pGame->Player->GetVelocitiesPtr() - vecRight * pGame->Player->GetSpeed());
		moveFlag = true;
	}

	if (InputEngine->IsKeyDown(DIK_D))
	{
		pGame->Player->SetVelocities(*pGame->Player->GetVelocitiesPtr() + vecRight * pGame->Player->GetSpeed());
		moveFlag = true;
	}

	if (moveFlag)
		pGame->GameEventHandler->onPlayerMoves(dt);

	if (InputEngine->IsKeyDown(DIK_SPACE))
		if (!PhysicsEngine->IsPlayerFree() && PhysicsEngine->IsPlayerCollidesWithWorld())
			pGame->Player->GetVelocitiesPtr()->z += 5.0f;

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

	if (InputEngine->IsMouseMoved())
	{
		MouseInfo mouseInfo = InputEngine->GetMouseInfo();
		pScene->GetActiveCamera()->LookLeftRight(-mouseInfo.deltaX * 0.2f);
		pScene->GetActiveCamera()->LookUpDown(-mouseInfo.deltaY * 0.2f);
	}

	if(InputEngine->IsMouseUp(LButton))
		pGame->GameEventHandler->onPlayerUsesItem(false);

	//if(InputEngine->IsMouseUp(LButton))
	//{
	//	Vector3D vec = pGame->Player->GetSelectedBlockPtr()->GetPositions().solid;
	//	if(vec.z > -1) //z не может быть меньше 0
	//	{
	//		vec3<int> pos = vec3<int>(floor(vec.x), floor(vec.y), floor(vec.z));
	//		auto block = pGame->World->GetDataStorage()->GetBlock(pos.x, pos.y, pos.z)->TypeID;

	//		if(pGame->World->RemoveBlock(pos.x, pos.y, pos.z))
	//			pGame->GameEventHandler->onBlockRemoved(block, pos);
	//	}
	//}

	if (InputEngine->IsMouseUp(RButton))
		pGame->GameEventHandler->onPlayerUsesItem(true);

}
