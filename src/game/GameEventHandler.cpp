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
	bool result = LostIsland::GetGamePtr()->Player->GetInventoryPtr()->AddItem(itemId, count);
	LostIsland::GetGamePtr()->gameHud->OnUserInventoryChange();
	return result;
}

void GameEventHandler::onPlayerDropsItem(unsigned slot, uint64_t itemId, size_t count)
{
	LostIsland::GetGamePtr()->Player->GetInventoryPtr()->RemoveItem(slot);
	auto pos = LostIsland::GetGamePtr()->Player->GetPosition();
	pos.z += 10.0f;
	LostIsland::GetGamePtr()->EntityMgr->CreateItemEntity(pos, Vector3D(0.0, 0.0, 3.0), itemId, count);
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

void GameEventHandler::onLivingObjectDies(LiveGameObject * object)
{
	if (!object)
		return;

	LOG_INFO(object, " dies.");

	auto pos = object->GetPosition();
	pos.z += 3.0f;
	auto items = object->GetInventory()->GetItems();
	for (const auto & item : items)
	{
		if (!item.count)
			continue;

		LOG_INFO(object, " dies. Creating ", item.itemId, " object in count of ", item.count);
		Vector3D velocity(sin(rand() % 16), cos(rand() % 16), 1.0);
		velocity = MathLib::Normalize(velocity);
		velocity *= 6.0f;
		LostIsland::GetGamePtr()->EntityMgr->CreateItemEntity(pos, velocity, item.itemId, item.count);
	}
}
