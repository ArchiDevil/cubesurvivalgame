#pragma once

#include "Item.h"

class Item;

class MiscItem : public Item
{
public:
	MiscItem(ItemUsingsHandler * _handler,
		const std::string & _Name,
		const std::string & _Desc,
		ShiftEngine::MeshDataPtr data,
		ShiftEngine::TexturePtr ptr)
		: Item(_handler, ptr, data, _Name, _Desc) {}

	~MiscItem() {}

	bool Use() 
	{
		return true;
	}

private:

};