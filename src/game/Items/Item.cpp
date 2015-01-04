#include "Item.h"

Item::Item(ShiftEngine::TexturePtr icon, ShiftEngine::MeshDataPtr mesh, 
		   const std::string & name, const std::string & desc)
	: name(name)
	, description(desc)
	, icon(icon)
	, mesh(mesh)
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
