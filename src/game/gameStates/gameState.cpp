#include "GameState.h"

#include <GraphicsEngine/ShiftEngine.h>
#include <SimplePhysicsEngine/PhysicsEngine.h>
#include <Utilities/inputConverter.h>

#include "../world/world.h"
#include "../Items/ItemManager.h"
#include "../gameHud.h"
#include "../Entities/GameObjectsManager.h"
#include "../Environment/EnvironmentManager.h"
#include "../CraftingManager.h"

//TEMPORARY
const float minR = 15.0f;
const float maxR = 60.0f;
float phi = 0.0f;
float theta = -35.0f;
float r = 20.0f;
//END OF TEMPORARY

GameState::GameState(IniWorker * iw, MyGUI::Gui * guiModule, MyGUI::DirectX11Platform * guiPlatform)
    : iniLoader(iw)
    , console()
    , guiModule(guiModule)
    , guiPlatform(guiPlatform)
{
}

GameState::~GameState()
{
}

bool GameState::initState()
{
    // to receive events for GUI
    subscribe(&InputEngine::GetInstance());

    LostIsland::CreateGame();

    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
    Game * pGame = LostIsland::GetGamePtr();

    console.subscribe(&InputEngine::GetInstance());

    ::utils::filesystem::CreateDir(L"saves/worlds/");
    ::utils::filesystem::CreateDir(L"saves/players/");
    ::utils::filesystem::CreateDir(L"saves/worlds/tempWorld/");

    int ChunksPerSide = iniLoader->GetInteger("ChunksPerSide");
    pGame->world->Initialize(ChunksPerSide, 0, 0, "tempWorld");
    LOG_INFO("World Manager has been initialized");

    SimplePhysicsEngine::GetInstance().Initialize(pGame->world->GetDataStorage());
    LOG_INFO("Physics initialized");

    pGame->itemMgr->Initialize("resources/gamedata/Items/");
    LOG_INFO("Items have been loaded");

    pGame->craftingMgr->Initialize("resources/gamedata/crafting/");
    LOG_INFO("Crafting manager has been initialized");

    pGame->gameHud.reset(new GameHUD(guiModule));
    LOG_INFO("HUD has been created");

    pGame->player = pGame->entityMgr->CreatePlayer(Vector3F()).get();

    pGame->environmentMgr->Initialize(dayTimer(11, 00));
    pGame->entityMgr->LoadEntities();

    pScene->AddCameraSceneNode();
    pScene->SetAmbientColor(Vector3F(0.1f, 0.1f, 0.15f));
    pScene->GetActiveCamera()->SetPosition({ 0.0f, 0.0f, 0.0f });
    pScene->GetActiveCamera()->RotateByQuaternion(MathLib::quaternionFromVecAngle(Vector3F(1.0f, 0.0f, 0.0f), degrad(-60.0f)));

    pGame->entityMgr->CreateEntity(Vector3F(-10.0, 10.0, 100.0), "stone");
    pGame->entityMgr->CreateEntity(Vector3F(10.0, 10.0, 100.0), "tree1");
    pGame->entityMgr->CreateItemEntity(Vector3F(-7.0f, 0.0f, 120.0f), Vector3F(), pGame->itemMgr->GetItemId("smooth_stone"), 10);

    LOG_INFO("End of game state initializing");

    return true;
}

bool GameState::update(double dt)
{
    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
    Game * pGame = LostIsland::GetGamePtr();

    ProcessInput(dt);
    pGame->gameHud->Update(dt);

    pGame->world->ProcessLoading();
    pGame->entityMgr->Update(dt);
    SimplePhysicsEngine::GetInstance().Update(dt);
    pGame->environmentMgr->Update(dt * 0.0);

    Vector3F playerPosition = pGame->player->GetPosition();
    ShiftEngine::CameraSceneNode * pCamera = pScene->GetActiveCamera();
    pCamera->SetSphericalCoords(playerPosition, phi, theta, r);

    return true;
}

bool GameState::render(double dt)
{
    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
    ShiftEngine::IContextManager * pCtxMgr = ShiftEngine::GetContextManager();
    ShiftEngine::Renderer * pRenderer = ShiftEngine::GetRenderer();
    Game * pGame = LostIsland::GetGamePtr();

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

    pCtxMgr->SetBlendingState(ShiftEngine::BlendingState::AlphaEnabled);

    pCtxMgr->BeginScene(); //no more needed here, cause clear frame should be called from renderer

    pScene->DrawAll(dt);

    pCtxMgr->SetZState(false);
    pGame->gameHud->Draw();
    pCtxMgr->SetZState(true);

    for (int i = 0; i < infoSize; i++)
        pFntMgr->DrawTextTL(di[i].str(), 0.0f, i * 32.0f);

    console.Draw();

    guiPlatform->getRenderManagerPtr()->drawOneFrame();

    pCtxMgr->EndScene();

    return true; //return false if something wrong
}

void GameState::onKill()
{
}

void GameState::onSuspend()
{
}

void GameState::onResume()
{
}

void GameState::ProcessInput(double dt)
{
    InputEngine & inputEngine = InputEngine::GetInstance();
    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
    ShiftEngine::IContextManager * pCtxMgr = ShiftEngine::GetContextManager();
    Game * pGame = LostIsland::GetGamePtr();
    inputEngine.GetKeys();
    auto mouseInfo = inputEngine.GetMouseInfo();

    if (inputEngine.IsKeyUp(DIK_1)) pGame->gameHud->SelectSlot(0);
    if (inputEngine.IsKeyUp(DIK_2)) pGame->gameHud->SelectSlot(1);
    if (inputEngine.IsKeyUp(DIK_3)) pGame->gameHud->SelectSlot(2);
    if (inputEngine.IsKeyUp(DIK_4)) pGame->gameHud->SelectSlot(3);
    if (inputEngine.IsKeyUp(DIK_5)) pGame->gameHud->SelectSlot(4);
    if (inputEngine.IsKeyUp(DIK_6)) pGame->gameHud->SelectSlot(5);
    if (inputEngine.IsKeyUp(DIK_7)) pGame->gameHud->SelectSlot(6);
    if (inputEngine.IsKeyUp(DIK_8)) pGame->gameHud->SelectSlot(7);
    if (inputEngine.IsKeyUp(DIK_9)) pGame->gameHud->SelectSlot(8);
    if (inputEngine.IsKeyUp(DIK_0)) pGame->gameHud->SelectSlot(9);

    if (inputEngine.IsKeyUp(DIK_C))
        pGame->gameHud->SwitchCraftingWindow();

    if (inputEngine.IsKeyUp(DIK_I))
        pGame->gameHud->SwitchInventoryWindow();

    if (inputEngine.IsKeyUp(DIK_GRAVE))
        console.SetVisibility(!console.IsVisible());

    if (console.IsVisible())
        return;

    if (inputEngine.IsKeyDown(DIK_ESCAPE))
        this->kill();

    if (inputEngine.IsKeyUp(DIK_V))
    {
        static bool Wflag = false;
        Wflag = !Wflag;
        if (Wflag)
            pCtxMgr->SetRasterizerState(ShiftEngine::RasterizerState::Wireframe);
        else
            pCtxMgr->SetRasterizerState(ShiftEngine::RasterizerState::Normal);
    }

    MyGUI::InputManager& inputManager = MyGUI::InputManager::getInstance();
    bool guiInjected = inputManager.injectMouseMove(mouseInfo.clientX, mouseInfo.clientY, 0);

    if (inputEngine.IsMouseDown(LButton))
        guiInjected |= inputManager.injectMousePress(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Left);

    if (inputEngine.IsMouseUp(LButton))
        guiInjected |= inputManager.injectMouseRelease(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Left);

    if (inputEngine.IsMouseDown(RButton))
        guiInjected |= inputManager.injectMousePress(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Right);

    if (inputEngine.IsMouseUp(RButton))
        guiInjected |= inputManager.injectMouseRelease(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Right);

    if (guiInjected)
        return;

    static size_t mousePath = 0;

    if (inputEngine.IsMouseMoved() && inputEngine.IsMouseDown(RButton))
    {
        theta -= (float)mouseInfo.deltaY * (float)dt * 10.0f;
        phi += (float)mouseInfo.deltaX * (float)dt * 10.0f;
        if (theta <= -35.0f)
            theta = -35.0f;
        if (theta >= -5.0f)
            theta = -5.0f;
    }

    ShiftEngine::GraphicEngineSettings settings = pCtxMgr->GetEngineSettings();
    vec2<unsigned int> sizes = vec2<unsigned int>(settings.screenWidth, settings.screenHeight);

    // do the raycasting
    mat4f projMatrix = pScene->GetActiveCamera()->GetProjectionMatrix();
    mat4f viewMatrix = pScene->GetActiveCamera()->GetViewMatrix();
    Vector3F resultNear = MathLib::getUnprojectedVector(Vector3F((float)mouseInfo.clientX, (float)mouseInfo.clientY, 0.0f), projMatrix, viewMatrix, sizes);
    Vector3F resultFar = MathLib::getUnprojectedVector(Vector3F((float)mouseInfo.clientX, (float)mouseInfo.clientY, 1.0f), projMatrix, viewMatrix, sizes);
    Ray unprojectedRay = Ray(resultNear, MathLib::normalize(resultFar - resultNear));

    pGame->entityMgr->HighlightEntity(unprojectedRay);

    if (inputEngine.IsMouseUp(RButton))
    {
        // check if we are targeting to entity
        auto retVal = pGame->entityMgr->GetNearestEntity(unprojectedRay);
        if (retVal)
        {
            InteractableGameObject* entity = (InteractableGameObject*)retVal.get();
            if (entity)
            {
                auto interaction = entity->GetInteraction();
                pGame->player->Interact(entity, interaction);
            }
        }
        else
        {
            if (mousePath <= 15)
            {
                Vector3F column = {};
                if (pGame->world->SelectColumnByRay(unprojectedRay, column))
                {
                    pGame->player->CancelCurrentCommand();
                    pGame->player->Go({ column.x, column.y });
                }
            }
        }
        mousePath = 0;
    }

    if (inputEngine.IsMouseDown(RButton))
        mousePath += abs(mouseInfo.deltaX) + abs(mouseInfo.deltaY);

    r -= (float)mouseInfo.deltaZ * (float)dt;
    r = MathLib::clamp(r, minR, maxR);
}

bool GameState::handleEvent(const InputEvent & event)
{
    MyGUI::InputManager& inputManager = MyGUI::InputManager::getInstance();

    switch (event.type)
    {
        // there will be always DirectInput keys in first two handlers
    case InputEventType::KeyDown:
        inputManager.injectKeyPress((MyGUI::KeyCode::Enum)event.key);
        break;
    case InputEventType::KeyUp:
        inputManager.injectKeyRelease((MyGUI::KeyCode::Enum)event.key);
        break;

        // there will be windows keys
    case InputEventType::SystemKey:
        inputManager.injectKeyPress((MyGUI::KeyCode::Enum)InputConverter::VirtualKeyToScanCode(event.key), event.key);
        break;
    }

    return true;
}
