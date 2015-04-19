#include "WorkState.h"

#include <GraphicsEngine/Renderer.h>
#include <Utilities/inputConverter.h>

float SensivityMultiplier = 0.2f;
static float x_angle = -45.0f;
static float y_angle = 45.0f;
static float R = 15.0f; //default camera radius
static float lenghtOfMove = 0;
const float threshold = 20.0f;

using namespace MeshEditor;

WorkState::WorkState(int x_size, int y_size, int z_size, MyGUI::Gui * pGui, MyGUI::DirectX11Platform * pPlatform)
    : flag(false)
    , geometryMode(true)
    , colorBox(L"gui/box.png")
    , curBrush({ 100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f })
    , pGui(pGui)
    , pPlatform(pPlatform)
{
    Workspace.reset(new BlockWorkspace(x_size, y_size, z_size));
}

WorkState::WorkState(const std::string & loadFile, MyGUI::Gui * pGui, MyGUI::DirectX11Platform * pPlatform)
    : WorkState{ 1, 1, 1, pGui, pPlatform }
{
    Workspace->Load(loadFile);
}

WorkState::~WorkState()
{
}

bool WorkState::initState()
{
    Workspace->Initialize();
    CreateGUI();
    int maxsize = Workspace->GetMaxSize();
    R = 3.0f * 1.4f * (0.5f * maxsize);
    return true;
}

bool WorkState::update(double dt)
{
    if (!ProcessInput(dt))
        return false;

    float x = R * sin(y_angle * 0.0175f) * cos(x_angle * 0.0175f);
    float y = R * sin(y_angle * 0.0175f) * sin(x_angle * 0.0175f);
    float z = R * cos(y_angle * 0.0175f);

    Vector3F addPos = Workspace->GetHalfSize();
    auto scg = ShiftEngine::GetSceneGraph();

    scg->GetActiveCamera()->SetPosition(Vector3F(x, y, z) + addPos);
    scg->GetActiveCamera()->LookAt(addPos - scg->GetActiveCamera()->GetPosition());

    static double tempCounter = 0.0;
    tempCounter += dt;
    if (tempCounter > 3.0)
    {
        Workspace->HideBoundingBox();
        tempCounter = 0.0;
    }

    if (flag)
    {
        auto & Input = InputEngine::GetInstance();
        newCoordinates = Vector2I(Input.GetMouseInfo().absoluteX, Input.GetMouseInfo().absoluteY);
        float distance = 0.0f;
        distance = (float)MathLib::distance(oldCoordinates, newCoordinates);
        oldCoordinates = newCoordinates;
        lenghtOfMove += distance;
    }

    Workspace->Update();
    colorBox.SetMaskColor({ curBrush.color.x, curBrush.color.y, curBrush.color.z, 1.0f });

    return true; //return false if something wrong
}

bool WorkState::render(double dt)
{
    auto t = InputEngine::GetInstance().GetMouseInfo();

    const int infoSize = 5;
    auto pContextManager = ShiftEngine::GetContextManager();
    auto pSceneGraph = ShiftEngine::GetSceneGraph();

    std::ostringstream di[infoSize];
    di[0] << "Alpha build number 216, for testing purposes only";
    if (geometryMode)
        di[1] << "Geometry";
    else
        di[1] << "Color";
    di[1] << " mode";
#if defined (DEBUG) || (_DEBUG)
    auto pRenderer = ShiftEngine::GetRenderer();
    di[2] << "FPS: " << pRenderer->GetFPS();
    di[3] << "Brush colors: " << curBrush.color.x << "/" << curBrush.color.y << "/" << curBrush.color.z;
    di[4] << "Polygons rendered: " << pRenderer->GetDrawnPolygonsCount();
#endif // DEBUG

    ////////////
    // RENDER //
    ////////////

    pContextManager->BeginScene();

    pSceneGraph->DrawAll(dt);
    auto font = pContextManager->GetFontManager()->GetCurrentFontName();
    pContextManager->GetFontManager()->SetFont(L"2");
    for (int i = 0; i < infoSize; i++)
        pContextManager->GetFontManager()->DrawTextTL(di[i].str(), 0.0f, 0.0f + i * 18.0f);

    pPlatform->getRenderManagerPtr()->drawOneFrame();

    if (!geometryMode)
        colorBox.Draw();

    pContextManager->GetFontManager()->SetFont(font);
    pContextManager->EndScene();

    return true; //return false if something wrong
}

void WorkState::onKill()
{
    pGui->destroyChildWidget(pSaveButton);
    pGui->destroyChildWidget(pColorButton);
    pGui->destroyChildWidget(pColorWindow);
    pGui->destroyChildWidget(pSaveWindow);
}

void WorkState::onSuspend()
{
    pColorWindow->setVisible(false);
    pSaveWindow->setVisible(false);
}

void WorkState::onResume()
{
}

void WorkState::CreateGUI()
{
    int screenWidth = ShiftEngine::GetContextManager()->GetEngineSettings().screenWidth;
    int screenHeight = ShiftEngine::GetContextManager()->GetEngineSettings().screenHeight;

    // save dialog

    pSaveWindow = pGui->createWidget<MyGUI::Window>("Window", MyGUI::IntCoord(screenWidth / 2 - 300 / 2, screenHeight / 2 - 130 / 2, 300, 130), MyGUI::Align::Default, "Overlapped");
    pSaveWindow->setCaption("Select name to save file");
    pSaveWindow->setVisible(false);
    pSaveWindow->setMovable(true);

    MyGUI::TextBox * pText = pSaveWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(5, 5, 270, 25), MyGUI::Align::Default);
    pText->setCaption("Filename: ");
    pText->setTextAlign(MyGUI::Align::HCenter);

    pSaveWindow->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(10, 30, 270, 25), MyGUI::Align::Default, "save_file_name");

    MyGUI::Button * pButton = pSaveWindow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(10, 60, 130, 25), MyGUI::Align::Default);
    pButton->setCaption("Ok");
    pButton->eventMouseButtonClick += MyGUI::newDelegate(this, &WorkState::SaveFile);

    pButton = pSaveWindow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(150, 60, 130, 25), MyGUI::Align::Default);
    pButton->setCaption("Cancel");
    pButton->eventMouseButtonClick += MyGUI::newDelegate(this, &WorkState::CancelSave);

    // color dialog

    pColorWindow = pGui->createWidget<MyGUI::Window>("Window", MyGUI::IntCoord(screenWidth / 2 - 300 / 2, screenHeight / 2 - 200 / 2, 300, 200), MyGUI::Align::Default, "Overlapped");
    pColorWindow->setCaption("Select color");
    pColorWindow->setVisible(false);
    pColorWindow->setMovable(true);

    pText = pColorWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(5, 5, 20, 16), MyGUI::Align::Default);
    pText->setCaption("R:");

    MyGUI::ScrollBar * pScroll = pColorWindow->createWidget<MyGUI::ScrollBar>("SliderH", MyGUI::IntCoord(30, 5, 250, 16), MyGUI::Align::Default, "red_scroll");
    pScroll->setColour(MyGUI::Colour(1.0f, 0.5f, 0.5f));
    pScroll->setScrollRange(256);
    pScroll->setScrollPosition((size_t)(curBrush.color.x * 255.0f));
    pScroll->eventScrollChangePosition += MyGUI::newDelegate(this, &WorkState::ColorChange);

    pText = pColorWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(5, 30, 20, 16), MyGUI::Align::Default);
    pText->setCaption("G:");

    pScroll = pColorWindow->createWidget<MyGUI::ScrollBar>("SliderH", MyGUI::IntCoord(30, 30, 250, 16), MyGUI::Align::Default, "green_scroll");
    pScroll->setColour(MyGUI::Colour(0.5f, 1.0f, 0.5f));
    pScroll->setScrollRange(256);
    pScroll->setScrollPosition((size_t)(curBrush.color.y * 255.0f));
    pScroll->eventScrollChangePosition += MyGUI::newDelegate(this, &WorkState::ColorChange);

    pText = pColorWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(5, 55, 20, 16), MyGUI::Align::Default);
    pText->setCaption("B:");

    pScroll = pColorWindow->createWidget<MyGUI::ScrollBar>("SliderH", MyGUI::IntCoord(30, 55, 250, 16), MyGUI::Align::Default, "blue_scroll");
    pScroll->setColour(MyGUI::Colour(0.5f, 0.5f, 1.0f));
    pScroll->setScrollRange(256);
    pScroll->setScrollPosition((size_t)(curBrush.color.z * 255.0f));
    pScroll->eventScrollChangePosition += MyGUI::newDelegate(this, &WorkState::ColorChange);

    MyGUI::Widget * pColorPanel = pColorWindow->createWidget<MyGUI::Widget>("PanelSkin", MyGUI::IntCoord(5, 80, 280, 25), MyGUI::Align::Default);
    pColorPanel->setAlpha(1.0f);
    pColorPanel->createWidget<MyGUI::Widget>("WhiteSkin", MyGUI::IntCoord(3, 3, 272, 17), MyGUI::Align::Stretch, "panel_color")->setColour(MyGUI::Colour(curBrush.color.x, curBrush.color.y, curBrush.color.z, 1.0f));

    pButton = pColorWindow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(5, 120, 280, 25), MyGUI::Align::Default);
    pButton->setCaption("Close");
    pButton->eventMouseButtonClick += MyGUI::newDelegate(this, &WorkState::CloseColorDialog);

    // other buttons

    pSaveButton = pGui->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(5, screenHeight - 30, 100, 25), MyGUI::Align::Default, "Overlapped");
    pSaveButton->setCaption("Save");
    pSaveButton->eventMouseButtonClick += MyGUI::newDelegate(this, &WorkState::ShowSaveDialog);

    pColorButton = pGui->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(120, screenHeight - 30, 100, 25), MyGUI::Align::Default, "Overlapped");
    pColorButton->setCaption("Color");
    pColorButton->eventMouseButtonClick += MyGUI::newDelegate(this, &WorkState::ShowColorDialog);
}

bool WorkState::ProcessInput(double dt)
{
    auto &inputEngine = InputEngine::GetInstance();
    inputEngine.GetKeys();
    auto mouseInfo = inputEngine.GetMouseInfo();

    bool guiInjected = false;

    MyGUI::InputManager& inputManager = MyGUI::InputManager::getInstance();
    guiInjected = inputManager.injectMouseMove(mouseInfo.clientX, mouseInfo.clientY, 0);

    if (inputEngine.IsMouseDown(LButton))
        guiInjected |= inputManager.injectMousePress(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Left);

    if (inputEngine.IsMouseUp(LButton))
        guiInjected |= inputManager.injectMouseRelease(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Left);

    if (inputEngine.IsMouseDown(RButton))
        guiInjected |= inputManager.injectMousePress(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Right);

    if (inputEngine.IsMouseUp(RButton))
        guiInjected |= inputManager.injectMouseRelease(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Right);

    if (guiInjected)
        return true;

    ShiftEngine::GraphicEngineSettings settings = ShiftEngine::GetContextManager()->GetEngineSettings();
    vec2<unsigned int> sizes = vec2<unsigned int>(settings.screenWidth, settings.screenHeight);

    Vector3F nearV, farV;
    mat4f viewMat = ShiftEngine::GetSceneGraph()->GetActiveCamera()->GetViewMatrix();
    mat4f projMat = ShiftEngine::GetSceneGraph()->GetActiveCamera()->GetProjectionMatrix();
    nearV = MathLib::getUnprojectedVector(Vector3F((float)mouseInfo.clientX, (float)mouseInfo.clientY, 0.0f), projMat, viewMat, sizes);
    farV = MathLib::getUnprojectedVector(Vector3F((float)mouseInfo.clientX, (float)mouseInfo.clientY, 1.0f), projMat, viewMat, sizes);
    Vector3F dir = MathLib::normalize(farV - nearV);

    if (geometryMode)
    {
        if (inputEngine.IsMouseUp(RButton))
        {
            if (lenghtOfMove < threshold)
            {
                Vector3F camPos = ShiftEngine::GetSceneGraph()->GetActiveCamera()->GetPosition();
                float distance = MathLib::distance(camPos, Vector3F());

                for (float mult = 0.0f; mult < distance*2.0f; mult += 0.1f)
                {
                    Vector3F temp = dir * mult + nearV;
                    Vector3I integerPos = { (int)floor(temp.x), (int)floor(temp.y), (int)floor(temp.z) };
                    if (Workspace->GetBlock(integerPos.x, integerPos.y, integerPos.z).exist)
                    {
                        Workspace->RemoveBlock(integerPos.x, integerPos.y, integerPos.z);
                        break;
                    }
                }
            }
            flag = false;
            lenghtOfMove = 0.0f;
        }

        if (inputEngine.IsMouseUp(LButton))
        {
            Vector3F camPos = ShiftEngine::GetSceneGraph()->GetActiveCamera()->GetPosition();
            float distance = MathLib::distance(camPos, Vector3F());
            //calculate new position
            const float step = 0.1f;
            for (float mult = 0.0f; mult < distance * 2.0f; mult += step)
            {
                Vector3F temp = dir * mult + nearV;
                Vector3I integerPos = { (int)floor(temp.x), (int)floor(temp.y), (int)floor(temp.z) };

                // this block only to check, block that would be modified must be got in other referenced variable!
                const Block & block = Workspace->GetBlock(integerPos.x, integerPos.y, integerPos.z);
                if (!block.exist)
                    continue;

                mult -= step;
                temp = dir * mult + nearV;
                integerPos = { (int)floor(temp.x), (int)floor(temp.y), (int)floor(temp.z) };
                auto workspaceSize = Workspace->GetHalfSize();
                workspaceSize *= 2.0f;
                if (temp.x >= 0.0f && temp.x <= workspaceSize.x &&
                    temp.y >= 0.0f && temp.y <= workspaceSize.y &&
                    temp.z >= 0.0f && temp.z <= workspaceSize.z)
                {
                    Workspace->AddBlock(integerPos.x, integerPos.y, integerPos.z);
                }
                else
                {
                    if ((int)floor(temp.z) == 0)
                    {
                        bool no = true;
                        if (Workspace->GetBlock(integerPos.x + 1, integerPos.y, integerPos.z).exist) no = false;
                        if (Workspace->GetBlock(integerPos.x - 1, integerPos.y, integerPos.z).exist) no = false;
                        if (Workspace->GetBlock(integerPos.x, integerPos.y + 1, integerPos.z).exist) no = false;
                        if (Workspace->GetBlock(integerPos.x, integerPos.y - 1, integerPos.z).exist) no = false;
                        if (no) break;
                    }

                    if (temp.x <= 0.0f)
                    {
                        Workspace->Resize(0, 0, 0, 1, 0, 0);
                        Workspace->AddBlock(integerPos.x + 1, integerPos.y, integerPos.z);
                    }

                    if (temp.y <= 0.0f)
                    {
                        Workspace->Resize(0, 0, 0, 0, 1, 0);
                        Workspace->AddBlock(integerPos.x, integerPos.y + 1, integerPos.z);
                    }

                    if (temp.x >= workspaceSize.x)
                    {
                        Workspace->Resize(1, 0, 0, 0, 0, 0);
                        Workspace->AddBlock(integerPos.x, integerPos.y, integerPos.z);
                    }

                    if (temp.y >= workspaceSize.y)
                    {
                        Workspace->Resize(0, 1, 0, 0, 0, 0);
                        Workspace->AddBlock(integerPos.x, integerPos.y, integerPos.z);
                    }

                    if (temp.z >= workspaceSize.z)
                    {
                        Workspace->Resize(0, 0, 1, 0, 0, 0);
                        Workspace->AddBlock(integerPos.x, integerPos.y, integerPos.z);
                    }
                }
                break;
            }
        }
    }
    else
    {
        if (inputEngine.IsMouseDown(LButton))
        {
            Vector3F camPos = ShiftEngine::GetSceneGraph()->GetActiveCamera()->GetPosition();
            float distance = MathLib::distance(camPos, Vector3F());

            for (float mult = 0.0f; mult < distance * 2.0f; mult += 0.1f)
            {
                Vector3F temp = dir * mult + nearV;
                if (Workspace->GetBlock((int)floor(temp.x), (int)floor(temp.y), (int)floor(temp.z)).exist == true)
                {
                    Workspace->SetBlockColor((int)floor(temp.x), (int)floor(temp.y), (int)floor(temp.z), curBrush.color);
                    break;
                }
            }
        }
    }

    if (inputEngine.IsMouseDown(RButton))
    {
        if (!flag)
            oldCoordinates = Vector2I(mouseInfo.absoluteX, mouseInfo.absoluteY);
        flag = true;
    }

    if (inputEngine.IsMouseMoved() && inputEngine.IsMouseDown(RButton))
    {
        //flag = true;
        if (lenghtOfMove > threshold)
        {
            x_angle -= mouseInfo.deltaX * SensivityMultiplier;
            if (y_angle - mouseInfo.deltaY * SensivityMultiplier < 179.0f &&
                y_angle - mouseInfo.deltaY * SensivityMultiplier > 1.0f)
            {
                y_angle -= mouseInfo.deltaY * SensivityMultiplier;
            }
        }
    }

    R += static_cast<float>(-mouseInfo.deltaZ * dt * SensivityMultiplier * 5);

    if (inputEngine.IsKeyUp(DIK_ESCAPE))
        this->kill();

    if (inputEngine.IsKeyUp(DIK_SPACE))
    {
        geometryMode = !geometryMode;
        if (geometryMode)
            MoveToGeometryMode();
        else
            MoveToColorMode();
    }

#if defined DEBUG || _DEBUG

    if (inputEngine.IsKeyDown(DIK_V))
        ShiftEngine::GetContextManager()->SetRasterizerState(ShiftEngine::RasterizerState::Wireframe);
    else if (inputEngine.IsKeyUp(DIK_V))
        ShiftEngine::GetContextManager()->SetRasterizerState(ShiftEngine::RasterizerState::Normal);

    static bool showBbox = false;

    if (inputEngine.IsKeyUp(DIK_F))
        showBbox = !showBbox;

    if (showBbox)
        Workspace->ShowBoundingBox();
    else
        Workspace->HideBoundingBox();

#endif

    if (inputEngine.IsKeyDown(DIK_LCONTROL) && inputEngine.IsKeyUp(DIK_Z))
        Workspace->Undo();

    return true;
}

void WorkState::MoveToGeometryMode()
{
    Workspace->VanishColor(true);
}

void WorkState::MoveToColorMode()
{
    Workspace->VanishColor(false);
}

void WorkState::SaveFile(MyGUI::Widget * _sender)
{
    MyGUI::EditBox * pEdit = pSaveWindow->findWidget("save_file_name")->castType<MyGUI::EditBox>();
    const std::wstring fileName = pEdit->getCaption().asWStr();

    const std::wstring extension = L".block";
    Workspace->Save(utils::Narrow(L"saves/" + fileName + extension));

    pSaveWindow->setVisible(false);
}

void WorkState::CancelSave(MyGUI::Widget * _sender)
{
    pSaveWindow->setVisible(false);
}

void WorkState::ShowSaveDialog(MyGUI::Widget * _sender)
{
    pSaveWindow->setVisible(true);
}

void WorkState::ShowColorDialog(MyGUI::Widget * _sender)
{
    pColorWindow->setVisible(true);
}

void WorkState::CloseColorDialog(MyGUI::Widget * _sender)
{
    pColorWindow->setVisible(false);
}

bool WorkState::handleEvent(const InputEvent & event)
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

void WorkState::ColorChange(MyGUI::ScrollBar* _sender, size_t newVal)
{
    if (_sender->getName() == "red_scroll")
        curBrush.color.x = (float)newVal / 255.0f;

    if (_sender->getName() == "green_scroll")
        curBrush.color.y = (float)newVal / 255.0f;

    if (_sender->getName() == "blue_scroll")
        curBrush.color.z = (float)newVal / 255.0f;

    MyGUI::Widget * pWidget = pColorWindow->findWidget("panel_color");
    if (pWidget)
        pWidget->setColour(MyGUI::Colour(curBrush.color.x, curBrush.color.y, curBrush.color.z));
}
