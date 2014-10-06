#include "MenuState.h"

MenuState::MenuState( SimpleGUI::Canvas * _pCanvas, SimpleGUI::Skinner * _pSkinner, Application * _pApp )
	: pCanvas(_pCanvas), pSkinner(_pSkinner), pApp(_pApp)
{
}

MenuState::~MenuState()
{
}

bool MenuState::initState()
{
	CreateGUI();
	return true;
}

bool MenuState::update( double dt )
{
	if(!ProcessInput())
		return false;
	return true;
}

bool MenuState::render( double dt )
{
	auto t = cInputEngine::GetInstance().GetMouseInfo();

	auto ge = ShiftEngine::GetContextManager();

	std::ostringstream di[1];
	di[0] << "Alpha build number 55, for testing purposes only";

	////////////
	// RENDER //
	////////////

	ge->BeginScene();
	auto font = ge->GetFontManager()->GetCurrentFontName();
	ge->GetFontManager()->SetFont(L"2");
	for (int i = 0; i < 1 ; i++)
		ge->GetFontManager()->DrawTextTL(di[i].str(), 0, 0 + i*18);

	pCanvas->Render(pSkinner);

	ge->GetFontManager()->SetFont(font);
	ge->EndScene();

	return true;
}

void MenuState::onKill(){}
void MenuState::onSuspend(){}
void MenuState::onResume(){}

void MenuState::CreateGUI()
{
	SimpleGUI::Image * back = new SimpleGUI::Image(pCanvas, L"MainMenuBack.png");
	back->SetSize(160, 280);
	back->SetPosition(200, 150);

	SimpleGUI::Text * create = new SimpleGUI::Text(back, "Create new block model");
	create->SetPosition(5, 10);

	//////////////////////////////////////////////////////////////////////////

	SimpleGUI::Textbox * tbX = new SimpleGUI::Textbox(back);
	tbX->SetSize(100, 20);
	tbX->SetPosition(40, 50);

	SimpleGUI::Text * x = new SimpleGUI::Text(back, "X: ");
	x->SetPosition(20, 50);

	//////////////////////////////////////////////////////////////////////////

	SimpleGUI::Textbox * tbY = new SimpleGUI::Textbox(back);
	tbY->SetSize(100, 20);
	tbY->SetPosition(40, 110);

	SimpleGUI::Text * y = new SimpleGUI::Text(back, "Y: ");
	y->SetPosition(20, 110);

	//////////////////////////////////////////////////////////////////////////

	SimpleGUI::Textbox * tbZ = new SimpleGUI::Textbox(back);
	tbZ->SetSize(100, 20);
	tbZ->SetPosition(40, 170);

	SimpleGUI::Text * z = new SimpleGUI::Text(back, "Z: ");
	z->SetPosition(20, 170);

	//////////////////////////////////////////////////////////////////////////

	SimpleGUI::Button * butNext = new SimpleGUI::Button(back);
	butNext->SetPosition(20, 230);
	butNext->SetSize(120, 20);
	butNext->SetText("Next");
	butNext->SetClickHandler(
		[=] (int mb, int innerX, int innerY)
		{
			if(utils::IsNumber(tbX->GetText()) && tbX->GetText() != L"" &&
				utils::IsNumber(tbY->GetText()) && tbY->GetText() != L"" &&
				utils::IsNumber(tbZ->GetText()) && tbZ->GetText() != L"")
				pApp->PushState(new WorkState(std::stoi(utils::WStrToStr(tbX->GetText())), 
				std::stoi(utils::WStrToStr(tbY->GetText())), 
				std::stoi(utils::WStrToStr(tbZ->GetText())), 
				pCanvas, 
				pSkinner));
		}
	);

	// creating load list

	SimpleGUI::List * loadingList = new SimpleGUI::List(pCanvas);
	loadingList->SetPosition(400, 150);
	loadingList->SetSize(160, 240);
	auto files = utils::filesystem::CollectFileNames(L"saves", L"block");
	for (int i = 0; i < files.size() ; i++)
	{
		loadingList->GetTable()->AddRow(utils::WStrToStr(files[i]));
	}

	SimpleGUI::Button * butLoad = new SimpleGUI::Button(pCanvas);
	butLoad->SetPosition(400, 410);
	butLoad->SetSize(160, 20);
	butLoad->SetText("Load");
	butLoad->SetClickHandler(
		[=] (int mb, int innerX, int innerY)
	{
		if(loadingList->GetTable()->GetSelectedRow() != nullptr && loadingList->GetTable()->GetSelectedRow()->GetString() != "")
		{
			pApp->PushState(new WorkState(utils::StrToWStr("saves/" + loadingList->GetTable()->GetSelectedRow()->GetString()),
				pCanvas,
				pSkinner));
		}
	}
	);
}

bool MenuState::ProcessInput()
{
	auto &InputEngine = cInputEngine::GetInstance();

	InputEngine.GetKeys();

	if(InputEngine.IsKeyDown(DIK_ESCAPE))
		return false;

	return true;
}
