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

class LiveBreed : public EntityBreed
{
public:
	LiveBreed(const std::string & meshName, const std::string & materialFile)
		: EntityBreed(meshName, materialFile)
	{}

	GameObjectPtr Clone() const override;
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
