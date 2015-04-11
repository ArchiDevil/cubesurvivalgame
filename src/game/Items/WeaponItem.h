#pragma once

#include "Item.h"

class Item;

class WeaponItem : public Item
{
public:
    WeaponItem(const std::string & name, 
        const std::string & desc, 
        ShiftEngine::IMeshDataPtr data,
        ShiftEngine::ITexturePtr ptr, 
        int damageCount,
        const std::string & imageName)
        : Item(ptr, data, name, desc, imageName)
        , damageCount(damageCount)
    {
    }

    bool UseOnPlayer() override
    {
        return false;
    }

    bool UseInWorld() override
    {
        return false;
    }

    ItemType GetType() const
    {
        return IT_Weapon;
    }

    int GetDamageCount() const
    {
        return damageCount;
    }

private:
    const int damageCount;

};
