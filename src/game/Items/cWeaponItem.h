#pragma once

#include "cItem.h"

class cItem;
class cItemUsingsHandler;

class cWeaponItem : public cItem
{
public:
	cWeaponItem(cItemUsingsHandler * _handler,
				const std::string & _Name,
				const std::string & _Desc,
				ShiftEngine::MeshDataPtr data,
				ShiftEngine::TexturePtr ptr,
				const int _damageCount = 0)
		: cItem(_handler, ptr, data, _Name, _Desc), 
		damageCount(_damageCount)
	{}

	~cWeaponItem() {}

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