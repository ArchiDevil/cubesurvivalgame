#pragma once

#include <cstdint>
#include <vector>

#include "../Items/ItemManager.h"

enum class ItemType;

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
    void RemoveItem(unsigned slot);

    bool IsExist(std::string & Name) const;
    bool IsExist(ItemType Type) const;
    bool IsExist(item_id_t itemId) const;

    SlotUnit GetItemInSlot(unsigned slot) const;
    const std::vector<SlotUnit> & GetItems() const;

    SlotUnit & FindSlotWithItem(item_id_t itemId);
    const SlotUnit & FindSlotWithItem(item_id_t itemId) const;

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

    void SetItemIntoRightHand(unsigned slot);

    //SlotUnit GetItemInLeftHand() const;
    SlotUnit GetItemInRightHand() const;

    //bool UseItemFromLeftHand();
    void RemoveItemFromRightHand();

private:
    SlotUnit rightHand;

};
