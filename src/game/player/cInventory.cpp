#include "cInventory.h"

cInventory::cInventory() {}

cInventory::~cInventory() {}

bool cInventory::IsExist( std::string & Name ) const
{
	for (int i = 0; i < InventorySize ; i++)
	{
		if(Items[i].Item->GetName() == Name)
		{
			return true;
		}
	}
	return false;
}

bool cInventory::IsExist( Item * it ) const
{
	for (auto iter = Items.begin(); iter != Items.end(); iter++)
		if((*iter).Item == it && (*iter).count > 0)
			return true;

	return false;
}

SlotUnit cInventory::GetItemInSlot( int slot ) const
{
	return Items[slot];
}

void cInventory::SetItemInSlot( int slot, Item * it )
{
	Items[slot].Item = it;
	Items[slot].count = 1;
}

int cInventory::GetCountFromSlot( int slot ) const
{
	return Items[slot].count;
}

void cInventory::SetCountInSlot( int slot, int count )
{
	Items[slot].count = count; 
}

SlotUnit cInventory::GetLeftHandItem() const
{
	return leftItemHand;
}

void cInventory::SetLeftItemHand( SlotUnit val )
{
	leftItemHand = val;
}

SlotUnit cInventory::GetRightHandItem() const
{
	return rightItemHand;
}

void cInventory::SetRightItemHand( SlotUnit val )
{
	rightItemHand = val;
}

int cInventory::GetFirstFreeSlotIndex() const
{
	for (int i = 0; i < InventorySize; i++)
		if(Items[i].count == 0)
			return i;

	return -1;
}

void cInventory::ResetEmpty()
{
	for (int i = 0; i < InventorySize ; i++)
		if(Items[i].count == 0)
			Items[i].Item = nullptr;
}

bool cInventory::AddItem( Item * it )
{
	//first we must find if this item exist in inventory

	for (int i = 0; i < InventorySize; i++)
	{
		if(Items[i].Item == it)
		{
			Items[i].count++;
			return true;
		}
	}

	//if not exist
	
	int slotIndex = GetFirstFreeSlotIndex();
	if(slotIndex != -1)
	{
		Items[slotIndex].Item = it;
		Items[slotIndex].count = 1;
		return true;
	}
	else
	{
		return false;
	}
}

bool cInventory::RemoveItem( Item * it )
{
	for (int i = 0; i < InventorySize; i++)
	{
		if(Items[i].Item == it)
		{
			if(--Items[i].count == 0)
			{
				Items[i].Item = nullptr;
			}
			return true;
		}
	}
	return false;
}

void cInventory::SetEmptyItem( Item * it )
{
	empty = it;
}
