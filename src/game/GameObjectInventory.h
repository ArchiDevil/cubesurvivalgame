#pragma once

#include <cstdint>
#include <vector>

class ItemManager;
enum ItemTypes;

struct SlotUnit
{
	SlotUnit(uint64_t itemId = 0, int count = 0) 
		: itemId(itemId)
		, count(count)
	{
	}

	uint64_t itemId;
	uint64_t count;
};

class GameObjectInventory
{
public:
	GameObjectInventory(ItemManager * pItemMgr, size_t inventorySize);
	~GameObjectInventory();

	bool AddItem(uint64_t itemId, size_t count);
	bool UseItem(unsigned slot);

	bool IsExist(std::string & Name) const;
	bool IsExist(ItemTypes Type) const;
	bool IsExist(uint64_t itemId) const;

	SlotUnit GetItemInSlot(int slot) const;

private:
	int GetFirstFreeSlotIndex() const;

	ItemManager * pItemMgr;
	std::vector<SlotUnit> Items;

};
