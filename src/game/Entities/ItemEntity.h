#pragma once

#include "PhysicsEntity.h"

class PhysicsEntity;

class ItemEntity : public PhysicsEntity
{
public:
	ItemEntity(uint64_t itemId, pPhysObject _obj, ShiftEngine::MeshNode * meshNode);
	~ItemEntity();

	uint64_t GetItemId() const;
	virtual void Update( double dt ) override;

private:
	uint64_t itemId;

};