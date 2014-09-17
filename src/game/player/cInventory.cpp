#include "cInventory.h"

#include "../Items/ItemManager.h"
#include "../Items/Item.h"

cInventory::cInventory(ItemManager * pItemMgr)
	: pItemMgr(pItemMgr)
{
}

cInventory::~cInventory()
{
}

bool cInventory::IsExist(std::string & Name) const
{
	for (int i = 0; i < InventorySize; i++)
		if (pItemMgr->GetItemById(Items[i].itemId)->GetName() == Name)
			return true;

	return false;
}

bool cInventory::IsExist(uint64_t itemId) const
{
	for (auto & item : Items)
		if (item.itemId == itemId)
			return true;

	return false;
}

SlotUnit cInventory::GetItemInSlot(int slot) const
{
	return Items[slot];
}

void cInventory::SetItemInSlot(int slot, uint64_t itemId)
{
	Items[slot].itemId = itemId;
	Items[slot].count = 1;
}

int cInventory::GetCountFromSlot(int slot) const
{
	return Items[slot].count;
}

void cInventory::SetCountInSlot(int slot, int count)
{
	Items[slot].count = count;
}

SlotUnit * cInventory::GetHandPtr()
{
	return &handItem;
}

int cInventory::GetFirstFreeSlotIndex() const
{
	for (int i = 0; i < InventorySize; i++)
		if (Items[i].count == 0)
			return i;

	return -1;
}

bool cInventory::AddItem(uint64_t itemId)
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

bool cInventory::RemoveItem(uint64_t itemId)
{
	for (auto & item : Items)
	{
		if (item.itemId == itemId)
		{
			if (--item.count == 0)
			{
				item.itemId = 0;
			}
			return true;
		}
	}
	return false;
}
