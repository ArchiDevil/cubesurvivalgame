#pragma once

#include "ToolItem.h"

class Item;
class ToolItem;

class FishingItem : public ToolItem
{
public:
    FishingItem(const std::string & name,
                const std::string & desc,
                ShiftEngine::IMeshDataPtr data,
                ShiftEngine::ITexturePtr ptr,
                const std::string & imageName)
                : ToolItem(name, desc, data, ptr, imageName)
    {
    }

    bool UseOnBlock(const Vector3F & blockPosition, BlockTypes blockType) override
    {
        return true;
    }

    virtual InteractionType GetInteractionWithBlock() const
    {
        return InteractionType::Fishing;
    }

};
