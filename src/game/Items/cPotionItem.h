#pragma once

#include "cItem.h"

class cItemUsingsHandler;

class cPotionItem : public cItem
{
public:
	cPotionItem(cItemUsingsHandler * _handler,
				const std::string & _Name,
				const std::string & _Desc,
				ShiftEngine::TexturePtr ptr,
				ShiftEngine::MeshDataPtr data,
				int _hp = 0,
				int _fp = 0,
				int _hnp = 0,
				int _hd = 0)
		: cItem(_handler, ptr, data, _Name, _Desc), healthPoints(_hp),
			fatiquePoints(_fp), hungerPoints(_hnp), healthDamage(_hd) {}

	~cPotionItem() {}

	bool Use() 
	{
		return handler->UsePotionItem(this);
	}

	int GetHealthPoints() const
	{
		return healthPoints;
	}

	int GetFatiquePoints() const
	{
		return fatiquePoints;
	}

	int GetHungerPoints() const
	{
		return hungerPoints;
	}

	int GetHealthDamage() const
	{
		return healthDamage;
	}

private:
	const int healthPoints;
	const int fatiquePoints;
	const int hungerPoints;

	const int healthDamage;

};