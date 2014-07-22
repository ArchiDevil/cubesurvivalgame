#pragma once

#include "cItem.h"

class cItem;

class cMiscItem : public cItem
{
public:
	cMiscItem(cItemUsingsHandler * _handler,
		const std::string & _Name,
		const std::string & _Desc,
		ShiftEngine::MeshDataPtr data,
		ShiftEngine::TexturePtr ptr)
		: cItem(_handler, ptr, data, _Name, _Desc) {}

	~cMiscItem() {}

	bool Use() 
	{
		return true;
	}

private:

};