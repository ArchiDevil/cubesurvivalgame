#include "GameEventHandler.h"

#include <MathLib/math.h>

#include "Entities/GameObjectInventory.h"
#include "game.h"

void GameEventHandler::onPlayerMoves(double /*dt*/)
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

bool GameEventHandler::onPlayerPicksItem(uint64_t itemId, size_t count)
{
	return LostIsland::GetGamePtr()->Player->GetInventoryPtr()->AddItem(itemId, count);
}

void GameEventHandler::onPlayerDropsItem(uint64_t itemId, size_t count)
{
	LostIsland::GetGamePtr()->Player->GetInventoryPtr()->RemoveItem(itemId);
	auto pos = LostIsland::GetGamePtr()->Player->GetPosition();
	pos.z += 10.0f;
	for (size_t i = 0; i < count; ++i)
		LostIsland::GetGamePtr()->EntityMgr->CreateItemEntity(pos, Vector3D(0.0, 0.0, 3.0), itemId);
}

void GameEventHandler::onPlayerUsesItem(bool self, item_id_t itemId)
{
	auto * pGame = LostIsland::GetGamePtr();
	auto * pItem = pGame->ItemMgr->GetItemById(itemId);
	if (!pItem)
		return;
	if (self)
		pItem->UseOnPlayer();
	else
		pItem->UseInWorld();
}

void GameEventHandler::onLivingObjectDies(ControllableGameObject * object, GameObjectInventory & inventory)
{
	if (!object)
		return;

	auto pos = object->GetPosition();
	pos.z += 10.0f;
	auto items = inventory.GetItems();
	for (const auto & item : items)
		for (size_t i = 0; i < item.count; ++i)
			LostIsland::GetGamePtr()->EntityMgr->CreateItemEntity(pos, Vector3D(0.0, 0.0, 3.0), item.itemId);
}
