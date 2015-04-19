#include "MenuState.h"

#include <Utilities/inputConverter.h>

MenuState::MenuState(Application * _pApp, MyGUI::Gui * pGui, MyGUI::DirectX11Platform * pPlatform)
    : pApp(_pApp)
    , pGui(pGui)
    , pPlatform(pPlatform)
{
}

MenuState::~MenuState()
{
}

bool MenuState::initState()
{
    subscribe(&InputEngine::GetInstance());

    CreateGUI();
    return true;
}

bool MenuState::update(double /*dt*/)
{
    if (!ProcessInput())
        return false;
    return true;
}

bool MenuState::render(double /*dt*/)
{
    auto ge = ShiftEngine::GetContextManager();

    std::ostringstream di;
    di << "Alpha build number 216, for testing purposes only";

    ////////////
    // RENDER //
    ////////////

    ge->BeginScene();
    auto font = ge->GetFontManager()->GetCurrentFontName();
    ge->GetFontManager()->SetFont(L"2");
    ge->GetFontManager()->DrawTextTL(di.str(), 0.0f, 18.0f);

    ge->GetFontManager()->SetFont(font);

    pPlatform->getRenderManagerPtr()->drawOneFrame();

    ge->EndScene();

    return true;
}

void MenuState::onKill()
{
    delete pWindow;
}

void MenuState::onSuspend()
{
    if (pWindow)
        pWindow->setVisible(false);
}

void MenuState::onResume()
{
    if (pWindow)
        pWindow->setVisible(true);
}

void MenuState::CreateGUI()
{
    auto & settings = ShiftEngine::GetContextManager()->GetEngineSettings();
    int xSize = 600;
    int ySize = 450;

    pWindow = pGui->createWidget<MyGUI::Window>("Window", MyGUI::IntCoord(settings.screenWidth / 2 - xSize / 2, settings.screenHeight / 2 - ySize / 2, xSize, ySize), MyGUI::Align::Default, "Overlapped");
    pWindow->setVisible(true);
    pWindow->setCaption("Start window");

    // left part

    MyGUI::TextBox * pTextBox = pWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(5, 5, 280, 25), MyGUI::Align::Center);
    pTextBox->setCaption("Create model");
    pTextBox->setTextAlign(MyGUI::Align::HCenter);

    MyGUI::EditBox* pEdit = pWindow->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(80, 30, 220, 24), MyGUI::Align::Default, "x_edit_box");
    pEdit->setEditMultiLine(true);

    pEdit = pWindow->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(80, 70, 220, 24), MyGUI::Align::Default, "y_edit_box");
    pEdit->setEditMultiLine(true);

    pEdit = pWindow->createWidget<MyGUI::EditBox>("EditBox", MyGUI::IntCoord(80, 110, 220, 24), MyGUI::Align::Default, "z_edit_box");
    pEdit->setEditMultiLine(true);

    pWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(5, 33, 70, 24), MyGUI::Align::Center)->setCaption("X size: ");
    pWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(5, 73, 70, 24), MyGUI::Align::Center)->setCaption("Y size: ");
    pWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(5, 113, 70, 24), MyGUI::Align::Center)->setCaption("Z size: ");

    MyGUI::Button * pButton = pWindow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(5, 150, 295, 20), MyGUI::Align::Default);
    pButton->setCaption("Create");
    pButton->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::Create);

    // right part

    pTextBox = pWindow->createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(310, 5, 280, 25), MyGUI::Align::Center);
    pTextBox->setCaption("Load model");
    pTextBox->setTextAlign(MyGUI::Align::HCenter);

    MyGUI::ListBox * pFileList = pWindow->createWidget<MyGUI::ListBox>("ListBox", MyGUI::IntCoord(310, 30, 280, 350), MyGUI::Align::Default, "file_list");
    auto files = utils::filesystem::CollectFileNames(L"saves", L"block");
    for (size_t i = 0; i < files.size(); i++)
        pFileList->addItem(files[i]);

    pButton = pWindow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(310, 385, 250, 20), MyGUI::Align::Default);
    pButton->setCaption("Load");
    pButton->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::LoadFile);

    pButton = pWindow->createWidget<MyGUI::Button>("Button", MyGUI::IntCoord(565, 385, 25, 20), MyGUI::Align::Default);
    pButton->setCaption("X");
    pButton->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::RemoveFile);
}

bool MenuState::ProcessInput()
{
    auto &inputEngine = InputEngine::GetInstance();
    inputEngine.GetKeys();
    auto mouseInfo = inputEngine.GetMouseInfo();

    MyGUI::InputManager& inputManager = MyGUI::InputManager::getInstance();

    inputManager.injectMouseMove(mouseInfo.clientX, mouseInfo.clientY, 0);

    if (inputEngine.IsMouseDown(LButton)) inputManager.injectMousePress(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Left);
    if (inputEngine.IsMouseUp(LButton)) inputManager.injectMouseRelease(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Left);

    if (inputEngine.IsMouseDown(RButton)) inputManager.injectMousePress(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Right);
    if (inputEngine.IsMouseUp(RButton)) inputManager.injectMouseRelease(mouseInfo.clientX, mouseInfo.clientY, MyGUI::MouseButton::Right);

    if (inputEngine.IsKeyDown(DIK_ESCAPE))
        return false;

    return true;
}

void MenuState::Create(MyGUI::Widget* _sender)
{
    size_t x = 0, y = 0, z = 0;

    auto text = pWindow->findWidget("x_edit_box")->castType<MyGUI::EditBox>()->getCaption();
    std::wstring t = text.asWStr();

    if (!utils::IsNumber(t))
        return;

    x = std::stoi(utils::Narrow(t));
    if (x > 20)
        x = 20;

    text = pWindow->findWidget("y_edit_box")->castType<MyGUI::EditBox>()->getCaption();
    t = text.asWStr();

    if (!utils::IsNumber(t))
        return;

    y = std::stoi(utils::Narrow(t));
    if (y > 20)
        y = 20;

    text = pWindow->findWidget("z_edit_box")->castType<MyGUI::EditBox>()->getCaption();
    t = text.asWStr();

    if (!utils::IsNumber(t))
        return;

    z = std::stoi(utils::Narrow(t));
    if (z > 20)
        z = 20;

    pApp->PushState(new WorkState(x, y, z, pGui, pPlatform));
}

void MenuState::LoadFile(MyGUI::Widget* _sender)
{
    MyGUI::ListBox * pList = pWindow->findWidget("file_list")->castType<MyGUI::ListBox>();
    size_t selectedFile = pList->getIndexSelected();
    std::wstring fileName = pList->getItemNameAt(selectedFile).asWStr();
    if (!fileName.empty())
    {
        std::string fullPath = "saves/" + utils::Narrow(fileName);
        pApp->PushState(new WorkState(fullPath, pGui, pPlatform));
    }
}

void MenuState::RemoveFile(MyGUI::Widget* _sender)
{
    MyGUI::ListBox * pList = pWindow->findWidget("file_list")->castType<MyGUI::ListBox>();
    size_t selectedFile = pList->getIndexSelected();
    std::wstring fileName = pList->getItemNameAt(selectedFile).asWStr();
    if (!fileName.empty())
    {
        std::string fullPath = "saves/" + utils::Narrow(fileName);
        utils::filesystem::RemoveFile(fullPath);
        pList->removeItemAt(selectedFile);
    }
}

bool MenuState::handleEvent(const InputEvent & event)
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
