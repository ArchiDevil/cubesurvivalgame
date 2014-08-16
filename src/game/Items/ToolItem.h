#pragma once

#include "Item.h"

class ToolItem : public Item
{
public:
	ToolItem(ItemUsingsHandler * _handler, 
			  const std::string & _Name, 
			  const std::string & _Desc,
			  ShiftEngine::MeshDataPtr data,
			  ShiftEngine::TexturePtr ptr)
		: Item(_handler, ptr, data, _Name, _Desc) {}

	~ToolItem() {}

	bool Use()
	{
		return false;
	}

};