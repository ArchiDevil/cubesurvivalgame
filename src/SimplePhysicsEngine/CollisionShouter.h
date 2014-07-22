#pragma once

#include <vector>

#include "types.h"

class CollisionListener
{
public:
	CollisionListener();
	~CollisionListener();

	std::vector<CollisionInfo> GetCollisionsPlayerEntityInfo() const;
	std::vector<CollisionInfo> & GetCollisionsPlayerEntityInfo();
	void ClearCollisions();
	void AddCollision(const CollisionInfo & col);

private:
	std::vector<CollisionInfo> CurrentCollisions;

};