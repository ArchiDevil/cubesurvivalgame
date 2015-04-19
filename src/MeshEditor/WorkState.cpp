#include "WorkState.h"

#include <GraphicsEngine/Renderer.h>

float SensivityMultiplier = 0.2f;
static float x_angle = -45.0f;
static float y_angle = 45.0f;
static float R = 15.0f; //default camera radius
static float lenghtOfMove = 0;
const float threshold = 20.0f;

using namespace MeshEditor;

WorkState::WorkState(int x_size, int y_size, int z_size)
    : flag(false)
    , geometryMode(true)
    , colorBox(L"gui/box.png")
    , curBrush({ 100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f })
{
    Workspace.reset(new BlockWorkspace(x_size, y_size, z_size));

    pCanvas = new SimpleGUI::Canvas;
    pSkinner = new SimpleGUI::Skinner;
    pSkinner->Initialize();
}

WorkState::WorkState(const std::string & loadFile)
    : WorkState(1, 1, 1)
{
    Workspace->Load(loadFile);
}

WorkState::~WorkState()
{
    delete pCanvas;
    delete pSkinner;
}

bool WorkState::initState()
{
    SimpleGUI::SetCanvas(pCanvas);
    SimpleGUI::SetSkinner(pSkinner);
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
        auto & Input = cInputEngine::GetInstance();
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
    auto t = cInputEngine::GetInstance().GetMouseInfo();

    const int infoSize = 5;
    auto pContextManager = ShiftEngine::GetContextManager();
    auto pSceneGraph = ShiftEngine::GetSceneGraph();

    std::ostringstream di[infoSize];
    di[0] << "Alpha build number 140, for testing purposes only";
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
        pContextManager->GetFontManager()->DrawTextTL(di[i].str(), 0, 0 + i * 18);

    SimpleGUI::DrawUI();

    if (!geometryMode)
        colorBox.Draw();

    pContextManager->GetFontManager()->SetFont(font);
    pContextManager->EndScene();

    return true; //return false if something wrong
}

void WorkState::onKill()
{
    SimpleGUI::SetCanvas(nullptr);
    SimpleGUI::SetSkinner(nullptr);
}

void WorkState::onSuspend()
{
}

void WorkState::onResume()
{
    SimpleGUI::SetCanvas(pCanvas);
    SimpleGUI::SetSkinner(pSkinner);
}

void WorkState::CreateGUI()
{
    int screenWidth = ShiftEngine::GetContextManager()->GetEngineSettings().screenWidth;
    int screenHeight = ShiftEngine::GetContextManager()->GetEngineSettings().screenHeight;
    /////////	
    SimpleGUI::Text * f = new SimpleGUI::Text(pCanvas, "Filename: ");
    f->SetPosition(50, screenHeight - 45);
    f->Hide();

    SimpleGUI::TextBox * fname = new SimpleGUI::TextBox(pCanvas);
    fname->SetSize(100, 20);
    fname->SetPosition(50, screenHeight - 25);
    fname->Hide();

    SimpleGUI::Button * butSaveOk = new SimpleGUI::Button(pCanvas);
    butSaveOk->SetPosition(170, screenHeight - 25);
    butSaveOk->SetSize(40, 20);
    butSaveOk->SetText("Ok");
    butSaveOk->Hide();

    SimpleGUI::Button * butSave = new SimpleGUI::Button(pCanvas);
    butSave->SetPosition(5, screenHeight - 45);
    butSave->SetSize(40, 40);
    butSave->SetText("Save");

    butSave->SetClickHandler(
        [=](MouseKeys, int, int)
    {
        f->Show();
        fname->Show();
        butSaveOk->Show();
    }
    );

    butSaveOk->SetClickHandler(
        [=](MouseKeys, int, int)
    {
        const std::string rr = ".block";
        Workspace->Save("saves/" + utils::Narrow(fname->GetText()) + rr);
        f->Hide();
        fname->Hide();
        butSaveOk->Hide();
        SimpleGUI::FocusedControl = nullptr;
    }
    );

    //////////////////////////////////////////////////////////////////////////

    // values boxes

    float panelWidth = 150;
    float panelHeight = 3 * 20 + 3 * 5; //(75)

    colorBox.SetPosition(Vector2F((float)screenWidth - 5.0f - panelWidth - 35.0f - panelHeight / 2 - 5, (float)screenHeight - panelHeight - 5.0f + 20 + 5 + 10));
    colorBox.SetSizeInPixels((int)panelHeight, (int)panelHeight);

    SimpleGUI::ValueBox * boxR = new SimpleGUI::ValueBox(pCanvas);
    boxR->SetPosition(screenWidth - 5 - (int)panelWidth - 35, screenHeight - (int)panelHeight - 5);
    boxR->SetSize((int)panelWidth, 20);
    boxR->SetMaxValue(255);
    boxR->SetValue(100);
    boxR->SetName("Name");
    boxR->Hide();

    SimpleGUI::Text * texR = new SimpleGUI::Text(pCanvas, "100");
    texR->SetPosition(screenWidth - 5 - 25, screenHeight - (int)panelHeight - 5);
    texR->SetName("Name");
    texR->Hide();

    boxR->SetOnValueChangedHandler([texR, boxR, this](int)
    {
        ColorsCallBack(texR, boxR);
        curBrush.color.x = boxR->GetValue() / 255.0f;
    });

    SimpleGUI::ValueBox * boxG = new SimpleGUI::ValueBox(pCanvas);
    boxG->SetPosition(screenWidth - 5 - (int)panelWidth - 35, screenHeight - (int)panelHeight - 5 + 20 + 5);
    boxG->SetSize((int)panelWidth, 20);
    boxG->SetMaxValue(255);
    boxG->SetValue(100);
    boxG->SetName("Name");
    boxG->Hide();

    SimpleGUI::Text * texG = new SimpleGUI::Text(pCanvas, "100");
    texG->SetPosition(screenWidth - 5 - 25, screenHeight - (int)panelHeight - 5 + 20 + 5);
    texG->SetName("Name");
    texG->Hide();

    boxG->SetOnValueChangedHandler([texG, boxG, this](int)
    {
        ColorsCallBack(texG, boxG);
        curBrush.color.y = boxG->GetValue() / 255.0f;
    });

    SimpleGUI::ValueBox * boxB = new SimpleGUI::ValueBox(pCanvas);
    boxB->SetPosition(screenWidth - 5 - (int)panelWidth - 35, screenHeight - (int)panelHeight - 5 + 20 + 5 + 20 + 5);
    boxB->SetSize((int)panelWidth, 20);
    boxB->SetMaxValue(255);
    boxB->SetValue(100);
    boxB->SetName("Name");
    boxB->Hide();

    SimpleGUI::Text * texB = new SimpleGUI::Text(pCanvas, "100");
    texB->SetPosition(screenWidth - 5 - 25, screenHeight - (int)panelHeight - 5 + 20 + 5 + 20 + 5);
    texB->SetName("Name");
    texB->Hide();

    boxB->SetOnValueChangedHandler([texB, boxB, this](int)
    {
        ColorsCallBack(texB, boxB);
        curBrush.color.z = boxB->GetValue() / 255.0f;
    });
}

bool WorkState::ProcessInput(double dt)
{
    auto &InputEngine = cInputEngine::GetInstance();

    InputEngine.GetKeys();

    if (SimpleGUI::HoveredControl != pCanvas)
        return true;

    auto mouseInfo = InputEngine.GetMouseInfo();

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
        if (InputEngine.IsMouseUp(RButton))
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

        if (InputEngine.IsMouseUp(LButton))
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
        if (InputEngine.IsMouseDown(LButton))
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

    if (InputEngine.IsMouseDown(RButton))
    {
        if (!flag)
            oldCoordinates = Vector2I(mouseInfo.absoluteX, mouseInfo.absoluteY);
        flag = true;
    }

    if (InputEngine.IsMouseMoved() && InputEngine.IsMouseDown(RButton))
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

    if (InputEngine.IsKeyUp(DIK_ESCAPE))
        this->kill();

    if (InputEngine.IsKeyUp(DIK_SPACE))
    {
        geometryMode = !geometryMode;
        if (geometryMode)
            MoveToGeometryMode();
        else
            MoveToColorMode();
    }

#if defined DEBUG || _DEBUG

    if (InputEngine.IsKeyDown(DIK_V))
        ShiftEngine::GetContextManager()->SetRasterizerState(ShiftEngine::RasterizerState::Wireframe);
    else if (InputEngine.IsKeyUp(DIK_V))
        ShiftEngine::GetContextManager()->SetRasterizerState(ShiftEngine::RasterizerState::Normal);

    static bool showBbox = false;

    if (InputEngine.IsKeyUp(DIK_F))
        showBbox = !showBbox;

    if (showBbox)
        Workspace->ShowBoundingBox();
    else
        Workspace->HideBoundingBox();

#endif

    if (InputEngine.IsKeyDown(DIK_LCONTROL) && InputEngine.IsKeyUp(DIK_Z))
        Workspace->Undo();

    return true;
}

void WorkState::MoveToGeometryMode()
{
    auto childs = pCanvas->GetChildrenList();
    for (auto iter = childs.cbegin(); iter != childs.cend(); iter++)
        if ((*iter)->GetName() != "")
            (*iter)->Hide();

    Workspace->VanishColor(true);
}

void WorkState::MoveToColorMode()
{
    auto childs = pCanvas->GetChildrenList();
    for (auto iter = childs.cbegin(); iter != childs.cend(); iter++)
        if ((*iter)->GetName() != "")
            (*iter)->Show();

    Workspace->VanishColor(false);
}

void WorkState::ColorsCallBack(SimpleGUI::Text * t, SimpleGUI::ValueBox * val)
{
    t->SetText(std::to_string(val->GetValue()));
}
