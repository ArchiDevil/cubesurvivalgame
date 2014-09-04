#pragma once

#include "Item.h"

class Item;

class EntityItem : public Item
{
public:
	EntityItem(ItemUsingsHandler * handler
		, const std::string & name
		, const std::string & desc
		, ShiftEngine::MeshDataPtr data
		, ShiftEngine::TexturePtr ptr
		, const std::string & entityId)
		: Item(handler, ptr, data, name, desc)
		, entityId(entityId)
	{
	}

	bool UseOnPlayer() override
	{
		return false;
	}

	bool UseInWorld() override
	{
		return handler->UseEntityItem(this);
	}

	ItemType GetType() const override
	{
		return IT_Entity;
	}

	const std::string & GetEntityId() const
	{
		return entityId;
	}

private:
	const std::string entityId;

};
