#pragma once

#include "PhysicsEntity.h"
#include "Items/cItem.h"

class cItem;
class PhysicsEntity;

class ItemEntity : public PhysicsEntity
{
	friend cEntityManager;

public:
	ItemEntity(cItem * _item, pPhysObject _obj, ShiftEngine::MeshNode * meshNode);
	~ItemEntity();

	cItem * GetItemPtr() const;

	virtual void Update( double dt );

private:
	cItem * item;

};