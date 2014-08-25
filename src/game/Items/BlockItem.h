#pragma once

#include "Item.h"
#include "../world/datatypes.h"

#include "../../GraphicsEngine/ShiftEngine.h"

class Item;
class ItemUsingsHandler;

class BlockItem : public Item
{
public:
	BlockItem(ItemUsingsHandler * _handler, 
			   const std::string & _Name, 
			   const std::string & _Desc,
			   ShiftEngine::TexturePtr ptr, 
			   BlockType _id)
		: Item(_handler, ptr, ShiftEngine::Utilities::createCube(), _Name, _Desc), BlockID(_id)
	{
		//mesh->SetScale(D3DXVECTOR3(0.4f, 0.4f, 0.4f)); //TEMPORARY COMMENTED
	}

	~BlockItem() {}

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