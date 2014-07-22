#pragma once

#include "cItem.h"

class cItem;
class cItemUsingsHandler;

enum bodyParts
{
	BP_head,
	BP_chest,
	BP_legs,
	BP_foots,
	BP_hands
};

class cArmorItem : public cItem
{
public:
	cArmorItem(cItemUsingsHandler * _handler,
			   const std::string & _Name,
			   const std::string & _Desc,
			   ShiftEngine::MeshDataPtr data,
			   ShiftEngine::TexturePtr ptr,
			   bodyParts _part,
			   const int _armorCount = 0,
			   const int _warmPoints = 0)
		: cItem(_handler, ptr, data, _Name, _Desc), 
		armorCount(_armorCount),
		warmPoints(_warmPoints),
		part(_part)
	{}

	~cArmorItem() {}

	int GetArmorCount() const
	{
		return armorCount;
	}

	int GetWarmPoints() const
	{
		return warmPoints;
	}

	bool Use()
	{
		return false;
	}
	
private:
	const int armorCount;
	const int warmPoints;
	bodyParts part;

};