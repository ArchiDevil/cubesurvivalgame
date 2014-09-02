#include "Item.h"

Item::Item( ItemUsingsHandler * _handler
		   , ShiftEngine::TexturePtr icon
		   , ShiftEngine::MeshDataPtr mesh
		   , const std::string & _Name
		   , const std::string & _Desc)
	: Name(_Name)
	, Description(_Desc)
	, icon(icon)
	, mesh(mesh)
	, handler(_handler)
{
}

Item::~Item()
{
}

std::string Item::GetName() const
{
	return Name;
}

std::string Item::GetDescription() const
{
	return Description;
}

ShiftEngine::TexturePtr Item::GetTexturePtr()
{
	return icon;
}

ShiftEngine::MeshDataPtr * Item::GetMesh()
{
	return &mesh;
}
