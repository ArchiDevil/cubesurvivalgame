#pragma once

#include <array>
#include <cstdint>

#define InventorySize 10

class Item;
class ItemManager;

enum ItemTypes;

struct SlotUnit
{
	SlotUnit(uint64_t itemId = 0, int count = 0) : itemId(itemId), count(count) {};
	uint64_t itemId;
	int count;
};

class cInventory
{
public:
	cInventory(ItemManager * pItemMgr);
	~cInventory();

	bool AddItem(uint64_t itemId);					//��������� ������� � ���������, true, ���� ����������, false, ���� ������ ��������
	bool RemoveItem(uint64_t itemId);				//������� ������� �� ���������

	int GetFirstFreeSlotIndex() const;				//������� ������ ��������� ���� � ���������� ��� ������

	bool IsExist(std::string & Name) const;			//��������� - ���������� �� � ��������� ������� � ����� ������
	bool IsExist(ItemTypes Type) const;				//��������� - ���������� �� � ��������� ������� ������ ����
	bool IsExist(uint64_t itemId) const;			//��������� - ���������� �� ������ �������� �������

	SlotUnit GetItemInSlot(int slot) const;			//������ ������� �� �����
	int GetCountFromSlot(int slot) const;			//������ ���������� ��������� � �����

	void SetItemInSlot(int slot, uint64_t itemId);	//������������� ������� � ����
	void SetCountInSlot(int slot, int count);		//������������� ���������� ��������� � �����

	SlotUnit GetLeftHandItem() const;				//���������� ������� �� ����� ����
	SlotUnit GetRightHandItem() const;				//���������� ������� �� ������ ����

	void SetLeftItemHand(SlotUnit val);				//������������� ������� � ����� ����
	void SetRightItemHand(SlotUnit val);			//������������� ������� � ������ ����

private:
	ItemManager * pItemMgr;

	std::array<SlotUnit, InventorySize> Items;

	SlotUnit leftItemHand;
	SlotUnit rightItemHand;

};