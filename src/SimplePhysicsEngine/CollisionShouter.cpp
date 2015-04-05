#include "CollisionShouter.h"

CollisionListener::CollisionListener()
{
}

CollisionListener::~CollisionListener()
{
}

const std::vector<CollisionInfo> & CollisionListener::GetCollisionsInfo() const
{
    return CurrentCollisions;
}

void CollisionListener::ClearCollisions()
{
    CurrentCollisions.clear();
}

void CollisionListener::AddCollision(const CollisionInfo & col)
{
    CollisionInfo ci = col;
    CurrentCollisions.push_back(ci);
}
