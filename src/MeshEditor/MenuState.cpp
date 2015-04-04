#include "MenuState.h"

MenuState::MenuState(Application * _pApp)
    : pCanvas(nullptr)
    , pSkinner(nullptr)
    , pApp(_pApp)
{
    pCanvas = new SimpleGUI::Canvas;
    pSkinner = new SimpleGUI::Skinner;
    pSkinner->Initialize();
}

MenuState::~MenuState()
{
    delete pCanvas;
    delete pSkinner;
}

bool MenuState::initState()
{
    SimpleGUI::SetCanvas(pCanvas);
    SimpleGUI::SetSkinner(pSkinner);

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
    auto t = cInputEngine::GetInstance().GetMouseInfo();
    auto ge = ShiftEngine::GetContextManager();

    std::ostringstream di[1];
    di[0] << "Alpha build number 140, for testing purposes only";

    ////////////
    // RENDER //
    ////////////

    ge->BeginScene();
    auto font = ge->GetFontManager()->GetCurrentFontName();
    ge->GetFontManager()->SetFont(L"2");
    for (int i = 0; i < 1; i++)
        ge->GetFontManager()->DrawTextTL(di[i].str(), 0, 0 + i * 18);

    SimpleGUI::DrawUI();

    ge->GetFontManager()->SetFont(font);
    ge->EndScene();

    return true;
}

void MenuState::onKill()
{
    SimpleGUI::SetCanvas(nullptr);
    SimpleGUI::SetSkinner(nullptr);
}

void MenuState::onSuspend()
{
}

void MenuState::onResume()
{
    SimpleGUI::SetCanvas(pCanvas);
    SimpleGUI::SetSkinner(pSkinner);
}

void MenuState::CreateGUI()
{
    SimpleGUI::Image * back = new SimpleGUI::Image(pCanvas, L"MainMenuBack.png");
    back->SetSize(160, 280);
    back->SetPosition(200, 150);

    SimpleGUI::Text * create = new SimpleGUI::Text(back, "Create new block model");
    create->SetPosition(5, 10);

    //////////////////////////////////////////////////////////////////////////

    SimpleGUI::TextBox * tbX = new SimpleGUI::TextBox(back);
    tbX->SetSize(100, 20);
    tbX->SetPosition(40, 50);
    tbX->SetText(L"8");

    SimpleGUI::Text * x = new SimpleGUI::Text(back, "X: ");
    x->SetPosition(20, 50);

    //////////////////////////////////////////////////////////////////////////

    SimpleGUI::TextBox * tbY = new SimpleGUI::TextBox(back);
    tbY->SetSize(100, 20);
    tbY->SetPosition(40, 110);
    tbY->SetText(L"8");

    SimpleGUI::Text * y = new SimpleGUI::Text(back, "Y: ");
    y->SetPosition(20, 110);

    //////////////////////////////////////////////////////////////////////////

    SimpleGUI::TextBox * tbZ = new SimpleGUI::TextBox(back);
    tbZ->SetSize(100, 20);
    tbZ->SetPosition(40, 170);
    tbZ->SetText(L"8");

    SimpleGUI::Text * z = new SimpleGUI::Text(back, "Z: ");
    z->SetPosition(20, 170);

    //////////////////////////////////////////////////////////////////////////

    SimpleGUI::Button * butNext = new SimpleGUI::Button(back);
    butNext->SetPosition(20, 230);
    butNext->SetSize(120, 20);
    butNext->SetText("Next");
    butNext->SetClickHandler(
        [=](MouseKeys /*mb*/, int /*innerX*/, int /*innerY*/)
    {
        if (utils::IsNumber(tbX->GetText()) && tbX->GetText() != L"" &&
            utils::IsNumber(tbY->GetText()) && tbY->GetText() != L"" &&
            utils::IsNumber(tbZ->GetText()) && tbZ->GetText() != L"")
            pApp->PushState(new WorkState(std::stoi(utils::Narrow(tbX->GetText())), std::stoi(utils::Narrow(tbY->GetText())), std::stoi(utils::Narrow(tbZ->GetText()))));
    }
    );

    // creating load list
    SimpleGUI::List * loadingList = new SimpleGUI::List(pCanvas);
    loadingList->SetPosition(400, 150);
    loadingList->SetSize(160, 240);
    auto files = utils::filesystem::CollectFileNames(L"saves", L"block");
    for (size_t i = 0; i < files.size(); i++)
    {
        loadingList->GetTable()->GetModel()->Add(utils::Narrow(files[i]));
    }

    SimpleGUI::Button * butLoad = new SimpleGUI::Button(pCanvas);
    butLoad->SetPosition(400, 410);
    butLoad->SetSize(160, 20);
    butLoad->SetText("Load");
    butLoad->SetClickHandler(
        [=](MouseKeys /*mb*/, int /*innerX*/, int /*innerY*/)
    {
        size_t selectedRow = loadingList->GetTable()->GetSelectedRow();
        if (selectedRow == (size_t)-1)
            return;

        std::string filename = loadingList->GetTable()->GetModel()->Get(selectedRow).GetName();
        if (!filename.empty())
        {
            std::string fullPath = "saves/" + filename;
            pApp->PushState(new WorkState(filename));
        }
    }
    );

    // SimpleGUI::Window * w = new SimpleGUI::Window(pCanvas);
    // w->SetPosition(100, 100);
    // w->SetSize(200, 100);
}

bool MenuState::ProcessInput()
{
    auto &InputEngine = cInputEngine::GetInstance();

    InputEngine.GetKeys();

    if (InputEngine.IsKeyDown(DIK_ESCAPE))
        return false;

    return true;
}
