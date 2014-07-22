#pragma once

#include "cItem.h"
#include "../GameObject.h"
#include "../world/datatypes.h"

#include "../../GraphicsEngine/ShiftEngine.h"

class cItem;
class cItemUsingsHandler;

class cBlockItem : public cItem
{
public:
	cBlockItem(cItemUsingsHandler * _handler, 
			   const std::string & _Name, 
			   const std::string & _Desc,
			   ShiftEngine::TexturePtr ptr, 
			   BlockType _id)
		: cItem(_handler, ptr, ShiftEngine::Utilities::createCube(), _Name, _Desc), BlockID(_id)
	{
		//mesh->SetScale(D3DXVECTOR3(0.4f, 0.4f, 0.4f)); //TEMPORARY COMMENTED
	}

	~cBlockItem() {}

	BlockType GetBlockID() const
	{
		return BlockID;
	}

	bool Use()
	{
		return handler->UseBlockItem(this);
	}
	
private:
	const BlockType BlockID;

};