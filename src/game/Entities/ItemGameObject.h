#pragma once

#include "PhysicsGameObject.h"

class PhysicsGameObject;

class ItemGameObject : public PhysicsGameObject
{
public:
	ItemGameObject(uint64_t itemId, PhysObjectPtr _obj, ShiftEngine::MeshNode * meshNode);
	~ItemGameObject();

	uint64_t GetItemId() const;
	virtual void Update(double dt) override;

private:
	uint64_t itemId;

};

typedef std::shared_ptr<ItemGameObject> ItemGameObjectPtr;
