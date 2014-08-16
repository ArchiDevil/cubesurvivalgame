#pragma once

#include "Item.h"

class FoodItem : public Item
{
public:
	FoodItem(ItemUsingsHandler * _handler, 
			  const std::string & _Name, 
			  const std::string & _Desc,
			  ShiftEngine::MeshDataPtr data,
			  ShiftEngine::TexturePtr ptr)
		: Item(_handler, ptr, data, _Name, _Desc) {}

	~FoodItem() {}

	bool Use()
	{
		//restore player's hunger
		return false;
	}

};