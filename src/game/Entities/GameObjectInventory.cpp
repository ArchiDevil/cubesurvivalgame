#include "GameObjectInventory.h"

#include "../Items/Item.h"

GameObjectInventory::GameObjectInventory(ItemManager * pItemMgr, size_t inventorySize)
    : pItemMgr(pItemMgr)
    , items(inventorySize)
{
}

bool GameObjectInventory::IsExist(std::string & Name) const
{
    for (const auto &it : items)
        if (pItemMgr->GetItemById(it.itemId)->GetName() == Name)
            return true;

    return false;
}

bool GameObjectInventory::IsExist(ItemType Type) const
{
    for (const auto &it : items)
        if (pItemMgr->GetItemById(it.itemId)->GetType() == Type)
            return true;

    return false;
}

bool GameObjectInventory::IsExist(item_id_t itemId) const
{
    for (const auto &item : items)
        if (item.itemId == itemId)
            return true;

    return false;
}

SlotUnit GameObjectInventory::GetItemInSlot(unsigned slot) const
{
    return items[slot];
}

int GameObjectInventory::GetFirstFreeSlotIndex() const
{
    for (size_t i = 0; i < items.size(); i++)
        if (items[i].count == 0)
            return i;

    return -1;
}

bool GameObjectInventory::AddItem(item_id_t itemId, size_t count)
{
    for (auto & item : items)
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
        items[slotIndex].itemId = itemId;
        items[slotIndex].count = count;
        return true;
    }
    else
    {
        return false;
    }
}

void GameObjectInventory::RemoveItem(unsigned slot)
{
    if (slot >= items.size())
        return;

    auto &itemSlot = items[slot];
    if (!itemSlot.itemId || !itemSlot.count)
        return;

    if (--itemSlot.count == 0)
        items[slot] = {};
}

const std::vector<SlotUnit> & GameObjectInventory::GetItems() const
{
    return items;
}

SlotUnit & GameObjectInventory::FindSlotWithItem(item_id_t itemId)
{
    static SlotUnit nullItem = SlotUnit();

    for (auto &item : items)
        if (item.itemId == itemId)
            return item;

    return nullItem;
}

const SlotUnit & GameObjectInventory::FindSlotWithItem(item_id_t itemId) const
{
    static SlotUnit nullItem = SlotUnit();

    for (auto &item : items)
        if (item.itemId == itemId)
            return item;

    return nullItem;
}

//////////////////////////////////////////////////////////

PlayerInventory::PlayerInventory(ItemManager * pItemMgr, size_t inventorySize)
    : GameObjectInventory(pItemMgr, inventorySize)
    , rightHand(0, 0)
{
}

bool PlayerInventory::AddItem(item_id_t itemId, size_t count)
{
    if (!itemId || !count)
        return false;

    if (rightHand.itemId == itemId)
        rightHand.count += count;

    if (!rightHand.itemId)
        rightHand = { itemId, count };

    return GameObjectInventory::AddItem(itemId, count);
}

void PlayerInventory::SetItemIntoRightHand(unsigned slot)
{
    SlotUnit u = GameObjectInventory::GetItemInSlot(slot);
    rightHand = u;
}

SlotUnit PlayerInventory::GetItemInRightHand() const
{
    return rightHand;
}

void PlayerInventory::RemoveItemFromRightHand()
{
    if (--rightHand.count == 0)
        rightHand = {};
}
