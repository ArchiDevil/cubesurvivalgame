#pragma once

#include "Item.h"

#include <stdexcept>

class Item;

// tools that can be used on blocks or other entities
// such as pickaxes, fishing rods, showels and etc.
// 
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

    virtual bool UseOnBlock(const MathLib::Vector3F & blockPosition, BlockTypes blockType)
    {
        throw std::runtime_error("You shouldn't be here!");
    }
};
