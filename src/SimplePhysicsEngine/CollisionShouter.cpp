#include "CollisionShouter.h"

CollisionListener::CollisionListener()
{
}

CollisionListener::~CollisionListener()
{
}

std::vector<CollisionInfo> CollisionListener::GetCollisionsPlayerEntityInfo() const
{
	return CurrentCollisions;
}

std::vector<CollisionInfo> & CollisionListener::GetCollisionsPlayerEntityInfo()
{
	return CurrentCollisions;
}

void CollisionListener::ClearCollisions()
{
	CurrentCollisions.clear();
}

void CollisionListener::AddCollision( const CollisionInfo & col )
{
	CollisionInfo ci = col;
	CurrentCollisions.push_back(ci);
}
