#pragma once

#include <cstdint>
#include <vector>

#include "Items/ItemManager.h"

enum ItemTypes;

struct SlotUnit
{
	SlotUnit(item_id_t itemId = 0, size_t count = 0)
		: itemId(itemId)
		, count(count)
	{
	}

	item_id_t itemId;
	size_t count;
};

class GameObjectInventory
{
public:
	GameObjectInventory(ItemManager * pItemMgr, size_t inventorySize);

	virtual bool AddItem(item_id_t itemId, size_t count);
	void DropItem(unsigned slot);
	void RemoveItem(unsigned slot);

	bool IsExist(std::string & Name) const;
	bool IsExist(ItemTypes Type) const;
	bool IsExist(uint64_t itemId) const;

	SlotUnit GetItemInSlot(unsigned slot) const;

protected:
	int GetFirstFreeSlotIndex() const;

	ItemManager * pItemMgr;
	std::vector<SlotUnit> items;

};

class PlayerInventory : public GameObjectInventory
{
public:
	PlayerInventory(ItemManager * pItemMgr, size_t inventorySize);
	virtual bool AddItem(item_id_t itemId, size_t count) override;

	//SlotUnit GetItemInLeftHand() const;
	SlotUnit GetItemInRightHand() const;

	//bool UseItemFromLeftHand();
	void RemoveItemFromRightHand();

private:
	SlotUnit rightHand;

};
