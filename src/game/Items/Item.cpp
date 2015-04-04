#include "Item.h"

Item::Item(ShiftEngine::TexturePtr icon, ShiftEngine::MeshDataPtr mesh,
           const std::string & name, const std::string & desc, const std::string & imageFile)
    : name(name)
    , description(desc)
    , icon(icon)
    , mesh(mesh)
    , itemImage(imageFile)
{
}

Item::~Item()
{
}

const std::string & Item::GetName() const
{
    return name;
}

const std::string & Item::GetDescription() const
{
    return description;
}

ShiftEngine::TexturePtr Item::GetTexturePtr() const
{
    return icon;
}

ShiftEngine::MeshDataPtr Item::GetMesh() const
{
    return mesh;
}

const std::string & Item::GetImageFile() const
{
    return itemImage;
}
