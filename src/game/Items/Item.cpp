#include "Item.h"

Item::Item( ItemUsingsHandler * _handler, ShiftEngine::TexturePtr ptr, ShiftEngine::MeshDataPtr data, 
			 const std::string & _Name /*= "NULL"*/, const std::string & _Desc /*= "NULL"*/ )
			 : Name(_Name), Description(_Desc), texture(ptr), mesh(data), handler(_handler)
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
	return texture;
}

ShiftEngine::MeshDataPtr * Item::GetMesh()
{
	return &mesh;
}
