#pragma once

#include "PhysicsGameObject.h"

class PhysicsGameObject;

class ItemGameObject : public PhysicsGameObject
{
public:
	ItemGameObject(uint64_t itemId, size_t count, PhysObjectPtr _obj, ShiftEngine::MeshNode * meshNode);
	~ItemGameObject();

	uint64_t GetItemId() const;
	virtual void Update(double dt) override;

private:
	uint64_t itemId;
	size_t count;

};

typedef std::shared_ptr<ItemGameObject> ItemGameObjectPtr;
