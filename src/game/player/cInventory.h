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

	bool AddItem(cItem * it);					//��������� ������� � ���������, true, ���� ����������, false, ���� ������ ��������
	bool RemoveItem(cItem * it);				//������� ������� �� ���������

	int GetFirstFreeSlotIndex() const;			//������� ������ ��������� ���� � ���������� ��� ������

	void ResetEmpty();							//���������� ��� ������� �������� � ������
	void SetEmptyItem(cItem * it);				//������������� ������� �������

	bool IsExist(std::string & Name) const;		//��������� - ���������� �� � ��������� ������� � ����� ������
	bool IsExist(ItemTypes Type) const;			//��������� - ���������� �� � ��������� ������� ������ ����
	bool IsExist(cItem * it) const;				//��������� - ���������� �� ������ �������� �������

	SlotUnit GetItemInSlot(int slot) const;		//������ ������� �� �����
	int GetCountFromSlot(int slot) const;		//������ ���������� ��������� � �����

	void SetItemInSlot(int slot, cItem * it);	//������������� ������� � ����
	void SetCountInSlot(int slot, int count);	//������������� ���������� ��������� � �����

	SlotUnit GetLeftHandItem() const;			//���������� ������� �� ����� ����
	SlotUnit GetRightHandItem() const;			//���������� ������� �� ������ ����

	void SetLeftItemHand(SlotUnit val);			//������������� ������� � ����� ����
	void SetRightItemHand(SlotUnit val);		//������������� ������� � ������ ����

private:
	array<SlotUnit, InventorySize> Items;

	cItem * empty;
	SlotUnit leftItemHand;
	SlotUnit rightItemHand;

};