#pragma once

#include "GameObject.h"

#include "StaticGameObject.h"
// #include "AnimalGameObject.h"
// #include "CollectableGameObject.h"
#include "ProducerGameObject.h"
#include "CrafterGameObject.h"
#include "ItemGameObject.h"

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

class ProducerBreed : public EntityBreed
{
public:
	ProducerBreed(const std::string & meshName, const std::string & materialFile,
				  Item * producedItem, uint32_t producingTime)
				  : EntityBreed(meshName, materialFile)
				  , producedItem(producedItem)
				  , producingTime(producingTime)
	{
	}

	GameObjectPtr Clone() const override;

protected:
	Item * producedItem;
	uint32_t producingTime;
};

class CrafterBreed : public EntityBreed
{
public:
	CrafterBreed(const std::string & meshName, const std::string & materialFile,
				 Item * craftingItem, uint32_t craftingTime)
				  : EntityBreed(meshName, materialFile)
				  , craftingItem(craftingItem)
				  , craftingTime(craftingTime)
	{
	}

	GameObjectPtr Clone() const override;

protected:
	Item * craftingItem;
	uint32_t craftingTime;
};

class StaticBreed : public EntityBreed
{
public:
	StaticBreed(const std::string & meshName, const std::string & materialFile)
		: EntityBreed(meshName, materialFile)
	{
	}

	GameObjectPtr Clone() const override;

protected:

};
