#include "cEventProc.h"

#include "cInventory.h"
#include "game.h"

void GameEventHandler::onPlayerMoves(double dt)
{
	static int PlayerCX = 0;
	static int PlayerCY = 0;
	static int prePlayerCX = PlayerCX;
	static int prePlayerCY = PlayerCY;

	auto pGame = LostIsland::GetGamePtr();

	prePlayerCX = PlayerCX;
	prePlayerCY = PlayerCY;

	PlayerCX = (int)floor(pGame->Player->GetPosition().x / (int)pGame->World->GetDataStorage()->GetChunkWidth());
	PlayerCY = (int)floor(pGame->Player->GetPosition().y / (int)pGame->World->GetDataStorage()->GetChunkWidth());

	if (PlayerCX != prePlayerCX)
	{
		if (PlayerCX - prePlayerCX < 0)
			pGame->World->ShiftChunkX(-1);
		if (PlayerCX - prePlayerCX > 0)
			pGame->World->ShiftChunkX(1);
	}

	if (PlayerCY != prePlayerCY)
	{
		if (PlayerCY - prePlayerCY < 0)
			pGame->World->ShiftChunkY(-1);
		if (PlayerCY - prePlayerCY > 0)
			pGame->World->ShiftChunkY(1);
	}
}

bool GameEventHandler::onPlayerPicksItem(uint64_t itemId)
{
	return LostIsland::GetGamePtr()->Player->GetInventoryPtr()->AddItem(itemId);
}

void GameEventHandler::onPlayerDropsItem(uint64_t itemId)
{
	if (!LostIsland::GetGamePtr()->Player->GetInventoryPtr()->RemoveItem(itemId))
	{
		MainLog.Error("Dropping item that doesn't exist");
		return;
	}

	auto pos = LostIsland::GetGamePtr()->Player->GetPosition();
	pos.z += 10.0f;
	LostIsland::GetGamePtr()->EntityMgr->CreateItemEntity(pos, Vector3D(0.0, 0.0, 3.0), itemId);
}

void GameEventHandler::onPlayerUsesItem(bool self)
{
	auto * pGame = LostIsland::GetGamePtr();
	SlotUnit * pHand = pGame->Player->GetInventoryPtr()->GetHandPtr();
	if (pHand->itemId)
	{
		auto * pItem = pGame->ItemMgr->GetItemById(pHand->itemId);
		bool useResult = false;
		if (self && pItem->UseOnPlayer())
			useResult = true;
		else if (!self && pItem->UseInWorld())
			useResult = true;
		if (useResult)
			if (--pHand->count == 0)
				pHand->itemId = 0;
	}
}
