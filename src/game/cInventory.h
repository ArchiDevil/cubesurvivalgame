#pragma once

#include <array>
#include <cstdint>

#define InventorySize 10

class Item;
class ItemManager;

enum ItemTypes;

struct SlotUnit
{
	SlotUnit(uint64_t itemId = 0, int count = 0) 
		: itemId(itemId), count(count)
	{}

	uint64_t itemId;
	int count;
};

class cInventory
{
public:
	cInventory(ItemManager * pItemMgr);
	~cInventory();

	bool AddItem(uint64_t itemId);					//добавляет предмет в инвентарь
	bool RemoveItem(uint64_t itemId);				//удаляет предмет из инвентаря

	int GetFirstFreeSlotIndex() const;				//находит первый свободный слот и возвращает его индекс

	bool IsExist(std::string & Name) const;			//проверяет - существует ли в инвентаре предмет с таким именем
	bool IsExist(ItemTypes Type) const;				//проверяет - существует ли в инвентаре предмет такого типа
	bool IsExist(uint64_t itemId) const;			//проверяет - существует ли вообще подобный предмет

	SlotUnit GetItemInSlot(int slot) const;			//отдает предмет из слота
	int GetCountFromSlot(int slot) const;			//отдает количество предметов в слоте

	void SetItemInSlot(int slot, uint64_t itemId);	//устанавливает предмет в слот
	void SetCountInSlot(int slot, int count);		//устанавливает количество предметов в слоте

	SlotUnit * GetHandPtr();						//возвращает предмет из левой руки

private:
	ItemManager * pItemMgr;
	std::array<SlotUnit, InventorySize> Items;
	SlotUnit handItem;
};
