#pragma once

#include "Item.h"

class Item;

class WeaponItem : public Item
{
public:
	WeaponItem(ItemUsingsHandler * handler
		, const std::string & name
		, const std::string & desc
		, ShiftEngine::MeshDataPtr data
		, ShiftEngine::TexturePtr ptr
		, int damageCount)
		: Item(handler, ptr, data, name, desc)
		, damageCount(damageCount)
	{
	}

	bool UseOnPlayer() override
	{
		return false;
	}

	bool UseInWorld() override
	{
		return handler->UseWeaponItem(this);
	}

	ItemType GetType() const
	{
		return IT_Weapon;
	}

	int GetDamageCount() const
	{
		return damageCount;
	}

private:
	const int damageCount;

};
