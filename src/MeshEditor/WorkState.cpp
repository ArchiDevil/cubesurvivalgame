#include "WorkState.h"

#include <GraphicsEngine/Renderer.h>

//implement geometry & color view

float SensivityMultiplier = 0.2f;
static float x_angle = -45.0f;
static float y_angle = 45.0f;
static float R = 15.0f;	//default camera radius
static float lenghtOfMove = 0;
const float threshold = 20.0f;

//0 Ч чЄрный				#000000
//1 Ч синий					#0000AA
//2 Ч зелЄный				#00AA00
//3 Ч сине-зелЄный			#00AAAA

//4 Ч красный				#AA0000
//5 Ч пурпурный				#AA00AA
//6 Ч коричневый			#AA5500
//7 Ч белый (светло-серый)	#AAAAAA

//8 Ч (тЄмно-) серый		#555555
//9 Ч голубой				#5555FF
//10 Ч €рко-зелЄный			#55FF55
//11 Ч €ркий сине-зелЄный	#55FFFF

//12 Ч €рко-красный			#FF5555
//13 Ч €рко-пурпурный		#FF55FF
//14 Ч жЄлтый				#FFFF55
//15 Ч €рко-белый			#FFFFFF

const float CGA_colors[16][3] = {
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.666f },
		{ 0.0f, 0.666f, 0.0f },
		{ 0.0f, 0.666f, 0.666f },

		{ 0.666f, 0.0f, 0.0f },
		{ 0.666f, 0.0f, 0.666f },
		{ 0.666f, 0.333f, 0.0f },
		{ 0.666f, 0.666f, 0.666f },

		{ 0.333f, 0.333f, 0.333f },
		{ 0.333f, 0.333f, 1.0f },
		{ 0.333f, 1.0f, 0.333f },
		{ 0.333f, 1.0f, 1.0f },

		{ 1.0f, 0.333f, 0.333f },
		{ 1.0f, 0.333f, 1.0f },
		{ 1.0f, 1.0f, 0.333f },
		{ 1.0f, 1.0f, 1.0f },
};

WorkState::WorkState(int x_size, int y_size, int z_size, SimpleGUI::Canvas * _pCanvas, SimpleGUI::Skinner * _pSkinner)
	: pCanvas(_pCanvas), pSkinner(_pSkinner), flag(false), geometryMode(true)
{
	Workspace = new BlockWorkspace(x_size, y_size, z_size);
	FillByDefault();
}

WorkState::WorkState(const std::wstring & loadFile, SimpleGUI::Canvas * _pCanvas, SimpleGUI::Skinner * _pSkinner)
	: pCanvas(_pCanvas), pSkinner(_pSkinner), flag(false), geometryMode(true)
{
	Workspace = new BlockWorkspace(1, 1, 1);
	Workspace->Load(loadFile);
}

WorkState::~WorkState()
{
	if (Workspace)
		delete Workspace;
}

bool WorkState::initState()
{
	pCanvas->RemoveAllChildrens();
	Workspace->Initialize();
	CreateGUI();
	int maxsize = Workspace->GetMaxSize();
	R = 3 * 1.4f * (0.5f * maxsize);
	return true;
}

bool WorkState::update(double dt)
{
	if (!ProcessInput(dt))
		return false;

	float x = R * sin(y_angle * 0.0175f) * cos(x_angle * 0.0175f);
	float y = R * sin(y_angle * 0.0175f) * sin(x_angle * 0.0175f);
	float z = R * cos(y_angle * 0.0175f);

	D3DXVECTOR3 addPos = D3DXVECTOR3(Workspace->GetHalfSize().x, Workspace->GetHalfSize().y, Workspace->GetHalfSize().z);

	auto scg = ShiftEngine::GetSceneGraph();

	scg->GetActiveCamera()->SetPosition(D3DXVECTOR3(x, y, z) + addPos);
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
		distance = MathLib::distance(oldCoordinates, newCoordinates);
		oldCoordinates = newCoordinates;
		lenghtOfMove += distance;
	}

	Workspace->Update();

	return true; //return false if something wrong
}

bool WorkState::render(double dt)
{
	auto t = cInputEngine::GetInstance().GetMouseInfo();

	const int infoSize = 7;
	auto pContextManager = ShiftEngine::GetContextManager();
	auto pRenderer = ShiftEngine::GetRenderer();
	auto pSceneGraph = ShiftEngine::GetSceneGraph();

	std::ostringstream di[infoSize];
	di[0] << "Alpha build number 71, for testing purposes only";
	if (geometryMode)
		di[1] << "Geometry";
	else
		di[1] << "Color";
	di[1] << " mode";
#if defined (DEBUG) || (_DEBUG)
	di[2] << "FPS: " << pRenderer->GetFPS();
	di[3] << "MousePos: " << t.clientX << "/" << t.clientY;
	di[4] << "CBColors: " << curBrush.Color.x << "/" << curBrush.Color.y << "/" << curBrush.Color.z;
	di[5] << "Mouse distance: " << lenghtOfMove;
	di[6] << "Polygons rendered: " << pRenderer->GetDrawnPolygonsCount();
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

	pCanvas->Render(pSkinner);
	pContextManager->GetFontManager()->SetFont(font);
	pContextManager->EndScene();

	return true; //return false if something wrong
}

void WorkState::onKill(){}
void WorkState::onSuspend(){}
void WorkState::onResume(){}

void WorkState::CreateGUI()
{
	int screenWidth = ShiftEngine::GetContextManager()->GetParameters().screenWidth;
	int screenHeight = ShiftEngine::GetContextManager()->GetParameters().screenHeight;
	/////////	
	SimpleGUI::Text * f = new SimpleGUI::Text(pCanvas, "Filename: ");
	f->SetPosition(50, screenHeight - 45);
	f->Hide();

	SimpleGUI::Textbox * fname = new SimpleGUI::Textbox(pCanvas);
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
		[=](int mb, int x, int y)
	{
		f->Show();
		fname->Show();
		butSaveOk->Show();
	}
	);

	butSaveOk->SetClickHandler(
		[=](int mb, int x, int y)
	{
		const std::wstring rr = L".block";
		Workspace->Save(L"saves/" + fname->GetText() + rr);
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

	SimpleGUI::ValueBox * boxR = new SimpleGUI::ValueBox(pCanvas);
	boxR->SetPosition(screenWidth - 5 - panelWidth - 35, screenHeight - panelHeight - 5);
	boxR->SetSize(panelWidth, 20);
	boxR->SetMaxValue(255);
	boxR->SetValue(100);
	boxR->SetName("Name");
	boxR->Hide();

	SimpleGUI::Text * texR = new SimpleGUI::Text(pCanvas, "100");
	texR->SetPosition(screenWidth - 5 - 25, screenHeight - panelHeight - 5);
	texR->SetName("Name");
	texR->Hide();

	boxR->SetOnValueChangedHandler([texR, boxR, this](int)
	{
		ColorsCallBack(texR, boxR);
		curBrush.Color.x = boxR->GetValue() / 255.0f;
	});

	SimpleGUI::ValueBox * boxG = new SimpleGUI::ValueBox(pCanvas);
	boxG->SetPosition(screenWidth - 5 - panelWidth - 35, screenHeight - panelHeight - 5 + 20 + 5);
	boxG->SetSize(panelWidth, 20);
	boxG->SetMaxValue(255);
	boxG->SetValue(100);
	boxG->SetName("Name");
	boxG->Hide();

	SimpleGUI::Text * texG = new SimpleGUI::Text(pCanvas, "100");
	texG->SetPosition(screenWidth - 5 - 25, screenHeight - panelHeight - 5 + 20 + 5);
	texG->SetName("Name");
	texG->Hide();

	boxG->SetOnValueChangedHandler([texG, boxG, this](int)
	{
		ColorsCallBack(texG, boxG);
		curBrush.Color.y = boxG->GetValue() / 255.0f;
	});

	SimpleGUI::ValueBox * boxB = new SimpleGUI::ValueBox(pCanvas);
	boxB->SetPosition(screenWidth - 5 - panelWidth - 35, screenHeight - panelHeight - 5 + 20 + 5 + 20 + 5);
	boxB->SetSize(panelWidth, 20);
	boxB->SetMaxValue(255);
	boxB->SetValue(100);
	boxB->SetName("Name");
	boxB->Hide();

	SimpleGUI::Text * texB = new SimpleGUI::Text(pCanvas, "100");
	texB->SetPosition(screenWidth - 5 - 25, screenHeight - panelHeight - 5 + 20 + 5 + 20 + 5);
	texB->SetName("Name");
	texB->Hide();

	boxB->SetOnValueChangedHandler([texB, boxB, this](int)
	{
		ColorsCallBack(texB, boxB);
		curBrush.Color.z = boxB->GetValue() / 255.0f;
	});
}

bool WorkState::ProcessInput(double ElapsedTime)
{
	auto &InputEngine = cInputEngine::GetInstance();

	InputEngine.GetKeys();

	auto mouseInfo = InputEngine.GetMouseInfo();

	Vector3F nearV, farV;
	mat4f viewMat = ShiftEngine::GetSceneGraph()->GetActiveCamera()->GetViewMatrix();
	mat4f projMat = ShiftEngine::GetSceneGraph()->GetActiveCamera()->GetProjectionMatrix();
	nearV = MathLib::getUnprojectedVector(Vector3F((float)mouseInfo.clientX, (float)mouseInfo.clientY, 0.0f), projMat, viewMat);
	farV = MathLib::getUnprojectedVector(Vector3F((float)mouseInfo.clientX, (float)mouseInfo.clientY, 1.0f), projMat, viewMat);
	Vector3F dir = MathLib::Normalize(farV - nearV);

	if (geometryMode)
	{
		if (InputEngine.IsMouseUp(LButton) && SimpleGUI::HoveredControl == pCanvas)
		{
			D3DXVECTOR3 camPos = ShiftEngine::GetSceneGraph()->GetActiveCamera()->GetPosition();
			float distance = MathLib::distance(Vector3F(camPos.x, camPos.y, camPos.z), Vector3F(0.0f, 0.0f, 0.0f));

			for (float mult = 0.0f; mult < distance*2.0f; mult += 0.05f)
			{
				Vector3F temp = dir * mult + nearV;
				if (Workspace->GetElem((int)floor(temp.x), (int)floor(temp.y), (int)floor(temp.z)).exist == true)
				{
					Workspace->SaveToUndo();
					Workspace->GetElem((int)floor(temp.x), (int)floor(temp.y), (int)floor(temp.z)).exist = false;
					break;
				}
			}
		}

		if (InputEngine.IsMouseUp(RButton))
		{
			if (lenghtOfMove < threshold)
			{
				D3DXVECTOR3 camPos = ShiftEngine::GetSceneGraph()->GetActiveCamera()->GetPosition();
				float distance = MathLib::distance(Vector3F(camPos.x, camPos.y, camPos.z), Vector3F());
				//calculate new position
				for (float mult = 0.0f; mult < distance * 2.0f; mult += 0.1f)
				{
					Vector3F temp = dir * mult + (nearV);
					Block & block = Workspace->GetElem((int)floor(temp.x), (int)floor(temp.y), (int)floor(temp.z));
					if (!block.exist)
						continue;

					mult -= 0.1f;
					temp = dir * mult + nearV;
					auto vector = Workspace->GetHalfSize();
					vector *= 2.0f;
					if (temp.x >= 0.0f && temp.x <= vector.x &&
						temp.y >= 0.0f && temp.y <= vector.y &&
						temp.z >= 0.0f && temp.z <= vector.z)
					{
						Workspace->SaveToUndo();
						block.exist = true;
						block.color = Vector3F(0.9f, 0.9f, 0.9f);
					}
					break;
				}
			}
			flag = false;
			lenghtOfMove = 0.0f;
		}
	}
	else
	{
		if (InputEngine.IsMouseUp(LButton))
		{
			D3DXVECTOR3 camPos = ShiftEngine::GetSceneGraph()->GetActiveCamera()->GetPosition();
			float distance = MathLib::distance(Vector3F(camPos.x, camPos.y, camPos.z), Vector3F(0.0f, 0.0f, 0.0f));

			for (float mult = 0.0f; mult < distance * 2.0f; mult += 0.05f)
			{
				Vector3F temp = dir * mult + nearV;
				if (Workspace->GetElem((int)floor(temp.x), (int)floor(temp.y), (int)floor(temp.z)).exist == true)
				{
					Workspace->SaveToUndo();
					Workspace->GetElem((int)floor(temp.x), (int)floor(temp.y), (int)floor(temp.z)).color = curBrush.Color;
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

		x_angle -= mouseInfo.deltaX * SensivityMultiplier;
		if (y_angle - mouseInfo.deltaY * SensivityMultiplier < 179.0f
			&& y_angle - mouseInfo.deltaY * SensivityMultiplier > 1.0f)
			y_angle -= mouseInfo.deltaY * SensivityMultiplier;
	}

	R += static_cast<float>(-mouseInfo.deltaZ * ElapsedTime * SensivityMultiplier * 5);

	if (InputEngine.IsKeyUp(DIK_ESCAPE))
	{
#if defined DEBUG || _DEBUG
		this->kill();
#endif
	}

	if (InputEngine.IsKeyUp(DIK_SPACE))
	{
		geometryMode = !geometryMode;
		if (geometryMode)
			MoveToGeometryMode();
		else
			MoveToColorMode();
	}

#if defined DEBUG || _DEBUG

	if(InputEngine.IsKeyDown(DIK_V))
		ShiftEngine::GetContextManager()->SetRasterizerState(ShiftEngine::RS_Wireframe);
	else if(InputEngine.IsKeyUp(DIK_V))
		ShiftEngine::GetContextManager()->SetRasterizerState(ShiftEngine::RS_Normal);

	static bool showBbox = false;

	if (InputEngine.IsKeyUp(DIK_F))
		showBbox = !showBbox;

	if(showBbox)
		Workspace->ShowBoundingBox();
	else
		Workspace->HideBoundingBox();

#endif

	if (InputEngine.IsKeyDown(DIK_LCONTROL) && InputEngine.IsKeyUp(DIK_Z))
		Workspace->Undo();

	return true;
}

void WorkState::FillByDefault()
{
	Block a;
	a.exist = true;
	a.color.x = 0.7f;
	a.color.y = 0.8f;
	a.color.z = 1.0f;

	//temporary!

	for (int i = 0; i < Workspace->GetHalfSize().x * 2; i++)
		for (int j = 0; j < Workspace->GetHalfSize().y * 2; j++)
			for (int k = 0; k < Workspace->GetHalfSize().z * 2; k++)
				Workspace->GetElem(i, j, k) = a;
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
