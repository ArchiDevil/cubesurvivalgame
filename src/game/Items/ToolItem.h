#pragma once

#include "Item.h"

#include <cassert>

class Item;

class ToolItem : public Item
{
public:
    ToolItem(const std::string & name,
             const std::string & desc,
             ShiftEngine::IMeshDataPtr data,
             ShiftEngine::ITexturePtr ptr,
             const std::string & imageName)
        : Item(ptr, data, name, desc, imageName)
    {
    }

    ItemType GetType() const override
    {
        return ItemType::Tool;
    }

    bool CanBeUsedOnBlock(BlockTypes type) const override
    {
        return true;
    }

    virtual bool UseOnBlock(const Vector3F & blockPosition, BlockTypes blockType)
    {
        assert(false);
    }
};
