#pragma once

#include "Item.h"

class Item;

class FoodItem : public Item
{
public:
	FoodItem(ItemUsingsHandler * _handler, 
			  const std::string & _Name, 
			  const std::string & _Desc,
			  ShiftEngine::MeshDataPtr data,
			  ShiftEngine::TexturePtr ptr,
			  int hunger)
		: Item(_handler, ptr, data, _Name, _Desc)
		, hunger(hunger)
	{
	}

	~FoodItem()
	{
	}

	bool UseOnPlayer() override
	{
		//restore player's hunger
		return false;
	}

	bool UseInWorld() override
	{
		return false;
	}

private:
	const int hunger;

};
