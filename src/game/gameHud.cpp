#include "gameHud.h"

#include "game.h"
#include "Entities/GameObjectInventory.h"
#include <GraphicsEngine/ShiftEngine.h>

gameHUD::gameHUD() 
	: liHandItem(nullptr)
	, liHandI(nullptr)
    , width(800)
    , height(600)
{
}

void gameHUD::Initialize( int sw, int sh )
{
	width = sw;
	height = sh;

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
	auto &pItemMgr = pGame->ItemMgr;
	SlotUnit handItem = pGame->Player->GetInventoryPtr()->GetItemInRightHand();

	if (handItem.itemId)
		liHandItem->SetTexture(pItemMgr->GetItemById(handItem.itemId)->GetTexturePtr());
	else
		liHandItem->SetTexture(nullptr);

	liHandI->Draw();
	liHandItem->Draw();
	ShiftEngine::GetContextManager()->GetFontManager()->DrawTextTL(std::to_string(handItem.count), width - 70, height - 70);
}
