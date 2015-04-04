#pragma once

#include "Item.h"

class Item;

class MiscItem : public Item
{
public:
    MiscItem(const std::string & name,
        const std::string & desc,
        ShiftEngine::MeshDataPtr data,
        ShiftEngine::TexturePtr ptr,
        const std::string & imageName)
        : Item(ptr, data, name, desc, imageName)
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

    ItemType GetType() const override
    {
        return IT_Misc;
    }
};
