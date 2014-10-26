#pragma once

#include "PhysicsGameObject.h"

class PhysicsGameObject;

class ItemEntity : public PhysicsGameObject
{
public:
	ItemEntity(uint64_t itemId, PhysObjectPtr _obj, ShiftEngine::MeshNode * meshNode);
	~ItemEntity();

	uint64_t GetItemId() const;
	virtual void Update( double dt ) override;

private:
	uint64_t itemId;

};
