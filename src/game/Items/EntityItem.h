#pragma once

#include "Item.h"

class Item;

class EntityItem : public Item
{
public:
    EntityItem(const std::string & name,
               const std::string & desc,
               ShiftEngine::IMeshDataPtr data,
               ShiftEngine::ITexturePtr ptr,
               const std::string & entityId,
               const std::string & imageName)
        : Item(ptr, data, name, desc, imageName)
        , entityId(entityId)
    {
    }

    ItemType GetType() const override
    {
        return ItemType::Entity;
    }

    const std::string & GetEntityId() const
    {
        return entityId;
    }

private:
    const std::string entityId = "";

};
