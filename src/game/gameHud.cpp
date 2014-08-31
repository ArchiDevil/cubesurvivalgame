#include "gameHud.h"

gameHUD::gameHUD() : crosshair(nullptr), liHandItem(nullptr),
	liHandI(nullptr), pPlayer(nullptr)
{
}

gameHUD::~gameHUD()
{
	delete crosshair;
	delete liHandItem;
	delete liHandI;
}

void gameHUD::Initialize( cPlayer * pPlayer, int sw, int sh )
{
	this->pPlayer = pPlayer;
	crosshair = new ShiftEngine::Sprite(L"gui/cross.png");
	crosshair->SetPosition(Vector2F((float)sw / 2, (float)sh / 2));

	liHandItem = new ShiftEngine::Sprite(L"");
	liHandItem->SetPosition(Vector2F(sw - 70.0, sh - 70.0));
	liHandItem->SetScale(Vector2F(1.4f, 1.4f));

	liHandI = new ShiftEngine::Sprite(L"gui/selectcube.png");
	liHandI->SetPosition(Vector2F(sw - 70.0, sh - 70.0));
	liHandI->SetScale(Vector2F(1.4f, 1.4f));
}

void gameHUD::Draw()
{
	//liHandItem->SetTexture(pPlayer->GetInventoryPtr()->GetLeftHandItem().Item->GetTexturePtr());
	liHandI->Draw();
	crosshair->Draw();
	//liHandItem->Draw();
}
