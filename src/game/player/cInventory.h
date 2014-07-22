#pragma once

#include <array>

#include "..\Items\cItem.h"

const int InventorySize = 10;

class cItem;
enum ItemTypes;

using std::array;

struct SlotUnit
{
	SlotUnit(cItem * ptr = nullptr, int _count = 0) : Item(ptr), count(_count) {};
	cItem * Item;
	int count;
};

class cInventory
{
public:
	cInventory();
	~cInventory();

	bool AddItem(cItem * it);					//добавляет предмет в инвентарь, true, если получилось, false, если нельзя добавить
	bool RemoveItem(cItem * it);				//удаляет предмет из инвентаря

	int GetFirstFreeSlotIndex() const;			//находит первый свободный слот и возвращает его индекс

	void ResetEmpty();							//сбрасывает все нулевые предметы в слотах
	void SetEmptyItem(cItem * it);				//устанавливает нулевой предмет

	bool IsExist(std::string & Name) const;		//проверяет - существует ли в инвентаре предмет с таким именем
	bool IsExist(ItemTypes Type) const;			//проверяет - существует ли в инвентаре предмет такого типа
	bool IsExist(cItem * it) const;				//проверяет - существует ли вообще подобный предмет

	SlotUnit GetItemInSlot(int slot) const;		//отдает предмет из слота
	int GetCountFromSlot(int slot) const;		//отдает количество предметов в слоте

	void SetItemInSlot(int slot, cItem * it);	//устанавливает предмет в слот
	void SetCountInSlot(int slot, int count);	//устанавливает количество предметов в слоте

	SlotUnit GetLeftHandItem() const;			//возвращает предмет из левой руки
	SlotUnit GetRightHandItem() const;			//возвращает предмет из правой руки

	void SetLeftItemHand(SlotUnit val);			//устанавливает предмет в левую руку
	void SetRightItemHand(SlotUnit val);		//устанавливает предмет в правую руку

private:
	array<SlotUnit, InventorySize> Items;

	cItem * empty;
	SlotUnit leftItemHand;
	SlotUnit rightItemHand;

};