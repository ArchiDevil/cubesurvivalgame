#pragma once

#include "Item.h"

#include "../Game.h"
#include "../Entities/GameObjectsManager.h"

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

    virtual bool UseInWorld(const MathLib::Vector3F & position) const override
    {
        return LostIsland::GetGamePtr()->entityMgr->CreateEntity(position, entityId) != nullptr;
    }

private:
    const std::string entityId = "";

};
