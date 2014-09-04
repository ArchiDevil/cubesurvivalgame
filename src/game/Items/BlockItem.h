#pragma once

#include "Item.h"
#include "../world/datatypes.h"

#include <GraphicsEngine/ShiftEngine.h>

class Item;

class BlockItem : public Item
{
public:
	BlockItem(ItemUsingsHandler * handler,
		const std::string & name,
		const std::string & desc,
		ShiftEngine::TexturePtr ptr,
		BlockType id)
		: Item(handler, ptr, ShiftEngine::Utilities::createCube(), name, desc)
		, BlockID(id)
	{
		//mesh->SetScale(D3DXVECTOR3(0.4f, 0.4f, 0.4f)); //TEMPORARY COMMENTED
	}

	BlockType GetBlockID() const
	{
		return BlockID;
	}

	bool UseOnPlayer() override
	{
		return false;
	}

	bool UseInWorld() override
	{
		return handler->UseBlockItem(this);
	}

	ItemType GetType() const override
	{
		return IT_Block;
	}

private:
	const BlockType BlockID;

};
