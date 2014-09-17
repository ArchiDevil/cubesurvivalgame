#include "gameHud.h"

#include "game.h"
#include "player/cInventory.h"

gameHUD::gameHUD() 
	: crosshair(nullptr)
	, liHandItem(nullptr)
	, liHandI(nullptr)
{
}

void gameHUD::Initialize( int sw, int sh )
{
	width = sw;
	height = sh;

	crosshair.reset(new ShiftEngine::Sprite(L"gui/cross.png"));
	crosshair->SetPosition(Vector2F((float)sw / 2, (float)sh / 2));

	liHandItem.reset(new ShiftEngine::Sprite(L""));
	liHandItem->SetPosition(Vector2F(sw - 70.0, sh - 70.0));
	liHandItem->SetScale(Vector2F(1.4f, 1.4f));

	liHandI.reset(new ShiftEngine::Sprite(L"gui/selectcube.png"));
	liHandI->SetPosition(Vector2F(sw - 70.0, sh - 70.0));
	liHandI->SetScale(Vector2F(1.4f, 1.4f));
}

void gameHUD::Draw()
{
	auto pGame = LostIsland::GetGamePtr();
	auto pItemMgr = pGame->ItemMgr;
	uint64_t itemId = pGame->Player->GetInventoryPtr()->GetHandPtr()->itemId;
	int itemCount = pGame->Player->GetInventoryPtr()->GetHandPtr()->count;

	if (itemId)
		liHandItem->SetTexture(pItemMgr->GetItemById(itemId)->GetTexturePtr());
	else
		liHandItem->SetTexture(nullptr);

	liHandI->Draw();
	crosshair->Draw();
	liHandItem->Draw();
	ShiftEngine::GetContextManager()->GetFontManager()->DrawTextTL(std::to_string(itemCount), width - 70, height - 70);
}
