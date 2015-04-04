#pragma once

#include "../Entities/GameObject.h"

#include <GraphicsEngine/D3D10Texture.h>
#include <GraphicsEngine/cMesh.h>

#include <string>

enum ItemType
{
    IT_Misc,
    IT_Weapon,
    IT_Entity,
    IT_Food,
    IT_Block
};

class Item
{
public:
    Item(ShiftEngine::TexturePtr icon,
        ShiftEngine::MeshDataPtr mesh,
        const std::string & name,
        const std::string & desc,
        const std::string & imageName);

    virtual ~Item();

    const std::string & GetDescription() const;
    const std::string & GetName() const;

    ShiftEngine::TexturePtr GetTexturePtr() const;
    ShiftEngine::MeshDataPtr GetMesh() const;

    const std::string & GetImageFile() const;

    virtual bool UseInWorld() = 0;
    virtual bool UseOnPlayer() = 0;
    // virtual bool UseOnGameObject(GameObject * target) = 0;

    virtual ItemType GetType() const = 0;

protected:
    const std::string name;
    const std::string description;
    const ShiftEngine::TexturePtr icon;
    ShiftEngine::MeshDataPtr mesh;
    const std::string itemImage;

};
