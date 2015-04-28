#pragma once

#include "Item.h"

class Item;

class MiscItem : public Item
{
public:
    MiscItem(const std::string & name,
             const std::string & desc,
             ShiftEngine::IMeshDataPtr data,
             ShiftEngine::ITexturePtr ptr,
             const std::string & imageName)
        : Item(ptr, data, name, desc, imageName)
    {
    }

    ItemType GetType() const override
    {
        return ItemType::Misc;
    }
};
