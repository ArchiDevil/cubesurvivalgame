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
				const int _damageCount = 0)
		: Item(_handler, ptr, data, _Name, _Desc), 
		damageCount(_damageCount)
	{}

	~WeaponItem() {}

	int GetDamageCount() const
	{
		return damageCount;
	}

	virtual bool Use() 
	{
		throw std::exception("The method or operation is not implemented.");
		return false;
	}


private:
	const int damageCount;

};