#include "cItem.h"

cItem::cItem( cItemUsingsHandler * _handler, ShiftEngine::TexturePtr ptr, ShiftEngine::MeshDataPtr data, 
			 const std::string & _Name /*= "NULL"*/, const std::string & _Desc /*= "NULL"*/ )
			 : Name(_Name), Description(_Desc), texture(ptr), mesh(data), handler(_handler)
{}

cItem::~cItem(){}

std::string cItem::GetName() const
{
	return Name;
}

std::string cItem::GetDescription() const
{
	return Description;
}

int cItem::GetDurability() const
{
	return durability;
}

void cItem::SetDurability( int _durability )
{
	durability = _durability;
}

ShiftEngine::TexturePtr cItem::GetTexturePtr()
{
	return texture;
}

ShiftEngine::MeshDataPtr * cItem::GetMesh()
{
	return &mesh;
}
