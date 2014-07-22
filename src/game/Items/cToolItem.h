#pragma once

#include "cItem.h"

class cToolItem : public cItem
{
public:
	cToolItem(cItemUsingsHandler * _handler, 
			  const std::string & _Name, 
			  const std::string & _Desc,
			  ShiftEngine::MeshDataPtr data,
			  ShiftEngine::TexturePtr ptr)
		: cItem(_handler, ptr, data, _Name, _Desc) {}

	~cToolItem() {}

	bool Use()
	{
		return false;
	}

};