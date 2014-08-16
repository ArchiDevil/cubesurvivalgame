#pragma once

#include "PhysicsEntity.h"
#include "Items/Item.h"

class Item;
class PhysicsEntity;

class ItemEntity : public PhysicsEntity
{
public:
	ItemEntity(Item * _item, pPhysObject _obj, ShiftEngine::MeshNode * meshNode);
	~ItemEntity();

	Item * GetItemPtr() const;

	virtual void Update( double dt );

private:
	Item * item;

};