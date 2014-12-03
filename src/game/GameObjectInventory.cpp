#include "GameObjectInventory.h"

#include "Items/ItemManager.h"
#include "Items/Item.h"

GameObjectInventory::GameObjectInventory(ItemManager * pItemMgr, size_t inventorySize)
	: pItemMgr(pItemMgr)
	, Items(inventorySize)
{
}

GameObjectInventory::~GameObjectInventory()
{
}

bool GameObjectInventory::IsExist(std::string & Name) const
{
	for (const auto &it : Items)
		if (pItemMgr->GetItemById(it.itemId)->GetName() == Name)
			return true;

	return false;
}

bool GameObjectInventory::IsExist(ItemTypes Type) const
{
	for (const auto &it : Items)
		if (pItemMgr->GetItemById(it.itemId)->GetType() == Type)
			return true;

	return false;
}

bool GameObjectInventory::IsExist(uint64_t itemId) const
{
	for (const auto &item : Items)
		if (item.itemId == itemId)
			return true;

	return false;
}

SlotUnit GameObjectInventory::GetItemInSlot(int slot) const
{
	return Items[slot];
}

int GameObjectInventory::GetFirstFreeSlotIndex() const
{
	for (size_t i = 0; i < Items.size(); i++)
		if (Items[i].count == 0)
			return i;

	return -1;
}

bool GameObjectInventory::AddItem(uint64_t itemId, size_t count)
{
	for (auto & item : Items)
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
		Items[slotIndex].itemId = itemId;
		Items[slotIndex].count = 1;
		return true;
	}
	else
	{
		return false;
	}
}

bool GameObjectInventory::UseItem(unsigned slot)
{
	if (slot >= Items.size())
		return false;

	auto &itemSlot = Items[slot];
	if (!itemSlot.itemId || !itemSlot.count)
		return false;

	auto *item = pItemMgr->GetItemById(itemSlot.itemId);
	if (item)
	{
		if (item->UseOnPlayer())
		{
			if (--itemSlot.count == 0)
			{
				// drop item
				Items[slot] = {};
				return true;
			}
		}
	}
	return false;
}
