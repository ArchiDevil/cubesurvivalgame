#pragma once

#include <array>

#include "..\Items\Item.h"

const int InventorySize = 10;

class Item;
enum ItemTypes;

using std::array;

struct SlotUnit
{
	SlotUnit(Item * ptr = nullptr, int _count = 0) : Item(ptr), count(_count) {};
	Item * Item;
	int count;
};

class cInventory
{
public:
	cInventory();
	~cInventory();

	bool AddItem(Item * it);					//��������� ������� � ���������, true, ���� ����������, false, ���� ������ ��������
	bool RemoveItem(Item * it);				//������� ������� �� ���������

	int GetFirstFreeSlotIndex() const;			//������� ������ ��������� ���� � ���������� ��� ������

	void ResetEmpty();							//���������� ��� ������� �������� � ������
	void SetEmptyItem(Item * it);				//������������� ������� �������

	bool IsExist(std::string & Name) const;		//��������� - ���������� �� � ��������� ������� � ����� ������
	bool IsExist(ItemTypes Type) const;			//��������� - ���������� �� � ��������� ������� ������ ����
	bool IsExist(Item * it) const;				//��������� - ���������� �� ������ �������� �������

	SlotUnit GetItemInSlot(int slot) const;		//������ ������� �� �����
	int GetCountFromSlot(int slot) const;		//������ ���������� ��������� � �����

	void SetItemInSlot(int slot, Item * it);	//������������� ������� � ����
	void SetCountInSlot(int slot, int count);	//������������� ���������� ��������� � �����

	SlotUnit GetLeftHandItem() const;			//���������� ������� �� ����� ����
	SlotUnit GetRightHandItem() const;			//���������� ������� �� ������ ����

	void SetLeftItemHand(SlotUnit val);			//������������� ������� � ����� ����
	void SetRightItemHand(SlotUnit val);		//������������� ������� � ������ ����

private:
	array<SlotUnit, InventorySize> Items;

	Item * empty;
	SlotUnit leftItemHand;
	SlotUnit rightItemHand;

};