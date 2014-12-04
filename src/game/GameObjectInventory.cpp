#include "GameObjectInventory.h"

#include "Items/ItemManager.h"
#include "Items/Item.h"

GameObjectInventory::GameObjectInventory(ItemManager * pItemMgr, size_t inventorySize)
	: pItemMgr(pItemMgr)
	, items(inventorySize)
{
}

bool GameObjectInventory::IsExist(std::string & Name) const
{
	for (const auto &it : items)
		if (pItemMgr->GetItemById(it.itemId)->GetName() == Name)
			return true;

	return false;
}

bool GameObjectInventory::IsExist(ItemTypes Type) const
{
	for (const auto &it : items)
		if (pItemMgr->GetItemById(it.itemId)->GetType() == Type)
			return true;

	return false;
}

bool GameObjectInventory::IsExist(uint64_t itemId) const
{
	for (const auto &item : items)
		if (item.itemId == itemId)
			return true;

	return false;
}

SlotUnit GameObjectInventory::GetItemInSlot(unsigned slot) const
{
	return items[slot];
}

int GameObjectInventory::GetFirstFreeSlotIndex() const
{
	for (size_t i = 0; i < items.size(); i++)
		if (items[i].count == 0)
			return i;

	return -1;
}

bool GameObjectInventory::AddItem(item_id_t itemId, size_t count)
{
	for (auto & item : items)
	{
		if (item.itemId == itemId)
		{
			++item.count;
			return true;
		}
	}

	int slotIndex = GetFirstFreeSlotIndex();
	if (slotIndex != -1)
	{
		items[slotIndex].itemId = itemId;
		items[slotIndex].count = 1;
		return true;
	}
	else
	{
		return false;
	}
}

void GameObjectInventory::DropItem(unsigned slot)
{
}

void GameObjectInventory::RemoveItem(unsigned slot)
{
	if (slot >= items.size())
		return;

	auto &itemSlot = items[slot];
	if (!itemSlot.itemId || !itemSlot.count)
		return;

	auto *item = pItemMgr->GetItemById(itemSlot.itemId);
	if (item)
		if (--itemSlot.count == 0)
			items[slot] = {};
}

//////////////////////////////////////////////////////////

PlayerInventory::PlayerInventory(ItemManager * pItemMgr, size_t inventorySize)
	: GameObjectInventory(pItemMgr, inventorySize)
{
}

bool PlayerInventory::AddItem(item_id_t itemId, size_t count)
{
	if (!itemId || !count)
		return false;

	if (rightHand.itemId == itemId)
		rightHand.count += count;

	if (!rightHand.itemId)
		rightHand = { itemId, count };

	return GameObjectInventory::AddItem(itemId, count);
}

SlotUnit PlayerInventory::GetItemInRightHand() const
{
	return rightHand;
}

void PlayerInventory::RemoveItemFromRightHand()
{
	auto *item = pItemMgr->GetItemById(rightHand.itemId);
	if (item)
		if (--rightHand.count == 0)
			rightHand = {};
}
