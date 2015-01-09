#pragma once

#include "GameObject.h"
#include "../Items/Item.h"
#include "../Items/ItemManager.h"

class EntityBreed
{
public:
	EntityBreed(const std::string & meshName, const std::string & materialFile) 
		: meshName(meshName)
		, materialFile(materialFile)
	{}

	virtual ~EntityBreed() {}
	virtual GameObjectPtr Clone() const = 0;

protected:
	std::string meshName;
	std::string materialFile;

};

typedef std::shared_ptr<EntityBreed> BreedPtr;

class InventoryBreed
{
public:
	struct ExtendedSlotUnit : public SlotUnit
	{
		ExtendedSlotUnit(item_id_t itemId = 0, size_t count = 0, size_t minimumCount = 0, size_t maximumCount = 0)
			: SlotUnit(itemId, count)
			, minimumCount(minimumCount)
			, maximumCount(maximumCount)
		{
		}

		size_t minimumCount;
		size_t maximumCount;
	};

	typedef std::vector<ExtendedSlotUnit> value_type;

	void AddItem(const ExtendedSlotUnit & item)
	{
		items.push_back(item);
	}

	void AddItem(ExtendedSlotUnit && item)
	{
		items.push_back(item);
	}

	const value_type & GetItems() const
	{
		return items;
	}

private:
	value_type items;
};

class LiveBreed : public EntityBreed
{
public:
	LiveBreed(const InventoryBreed & inventory, const std::string & meshName, const std::string & materialFile)
		: EntityBreed(meshName, materialFile)
		, inventoryBreedPtr(inventory)
	{
	}

	GameObjectPtr Clone() const override;

private:
	const InventoryBreed & inventoryBreedPtr;
};

class CollectableBreed : public EntityBreed
{
public:
	CollectableBreed(const std::string & meshName, const std::string & materialFile, item_id_t itemId, size_t count)
		: EntityBreed(meshName, materialFile)
		, itemId(itemId)
		, count(count)
	{}

	GameObjectPtr Clone() const override;

protected:
	item_id_t itemId;
	size_t count;
};
