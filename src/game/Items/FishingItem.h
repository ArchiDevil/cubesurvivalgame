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
                const std::string & imageName,
                unsigned fishingQuality)
        : ToolItem(name, desc, data, ptr, imageName)
        , fishingQuality(fishingQuality)
    {
    }

    bool UseOnBlock(const MathLib::Vector3F & blockPosition, BlockTypes blockType) override
    {
        return true;
    }

    virtual InteractionType GetInteractionWithBlock() const
    {
        return InteractionType::Fishing;
    }

    unsigned GetFishingQuality() const
    {
        return fishingQuality;
    }

    bool CanBeUsedOnBlock(BlockTypes type) const override
    {
        return type == BT_Water;
    }

    unsigned fishingQuality = 0;
};
