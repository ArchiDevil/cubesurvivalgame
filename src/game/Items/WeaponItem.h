#pragma once

#include "Item.h"

class Item;
class ItemUsingsHandler;

class WeaponItem : public Item
{
public:
	WeaponItem(ItemUsingsHandler * _handler,
				const std::string & _Name,
				const std::string & _Desc,
				ShiftEngine::MeshDataPtr data,
				ShiftEngine::TexturePtr ptr,
				int damageCount)
		: Item(_handler, ptr, data, _Name, _Desc)
		, damageCount(damageCount)
	{
	}

	~WeaponItem()
	{
	}

	int GetDamageCount() const
	{
		return damageCount;
	}

	bool UseOnPlayer() override
	{
		return false;
	}

	bool UseInWorld() override
	{
		throw std::exception("The method or operation is not implemented.");
		return false;
	}

private:
	const int damageCount;

};
