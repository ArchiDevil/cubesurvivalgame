#pragma once

#include "Item.h"

class Item;

class FoodItem : public Item
{
public:
	FoodItem(ItemUsingsHandler * handler
		, const std::string & name
		, const std::string & desc
		, ShiftEngine::MeshDataPtr data
		, ShiftEngine::TexturePtr ptr
		, int hunger)
		: Item(handler, ptr, data, name, desc)
		, hunger(hunger)
	{
	}

	bool UseOnPlayer() override
	{
		return handler->UseFoodItem(this);
	}

	bool UseInWorld() override
	{
		return false;
	}

	ItemType GetType() const override
	{
		return IT_Food;
	}

	int GetHunger() const
	{
		return hunger;
	}

private:
	const int hunger;

};
