#pragma once

#include "Item.h"

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

class ArmorItem : public Item
{
public:
	ArmorItem(ItemUsingsHandler * _handler,
			   const std::string & _Name,
			   const std::string & _Desc,
			   ShiftEngine::MeshDataPtr data,
			   ShiftEngine::TexturePtr ptr,
			   bodyParts _part,
			   const int _armorCount = 0,
			   const int _warmPoints = 0)
		: Item(_handler, ptr, data, _Name, _Desc), 
		armorCount(_armorCount),
		warmPoints(_warmPoints),
		part(_part)
	{}

	~ArmorItem() {}

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