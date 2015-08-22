#pragma once

#include "../Entities/LiveGameObject.h"
#include "../world/blockColumn.h"

#include <GraphicsEngine/ITexture.h>
#include <GraphicsEngine/IMeshData.h>

#include <string>

enum class ItemType
{
    Misc,
    Weapon,
    Entity,
    Food,
    Tool
};

class Item
{
public:
    Item(ShiftEngine::ITexturePtr icon,
         ShiftEngine::IMeshDataPtr mesh,
         const std::string & name,
         const std::string & desc,
         const std::string & imageName)
        : name(name)
        , description(desc)
        , icon(icon)
        , mesh(mesh)
        , itemImage(imageName)
    {
    }

    virtual ~Item()
    {
    }

    const std::string & GetDescription() const
    {
        return description;
    }

    const std::string & GetName() const
    {
        return name;
    }

    ShiftEngine::ITexturePtr GetTexturePtr() const
    {
        return icon;
    }

    ShiftEngine::IMeshDataPtr GetMesh() const
    {
        return mesh;
    }

    const std::string & GetImageFile() const
    {
        return itemImage;
    }

    // next calls are empty to simplify description of inherited items
    virtual bool UseInWorld(const MathLib::Vector3F & position) const
    {
        return false;
    }

    virtual bool CanBeUsedOnBlock(BlockTypes type) const
    {
        return false;
    }

    virtual InteractionType GetInteractionWithBlock() const
    {
        return InteractionType::Nothing;
    }

    virtual bool UseOnPlayer() const
    {
        return false;
    }

    virtual bool UseOnGameObject(LiveGameObject * target) const
    {
        return false;
    }

    virtual ItemType GetType() const = 0;

protected:
    const std::string name;
    const std::string description;
    const ShiftEngine::ITexturePtr icon;
    ShiftEngine::IMeshDataPtr mesh;
    const std::string itemImage;

};
