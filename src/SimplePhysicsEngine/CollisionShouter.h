#pragma once

#include <vector>
#include "types.h"

struct CollisionInfo
{
    CollisionInfo(PhysObjectPtr obj)
        : obj(obj)
    {
    }

    PhysObjectPtr obj;
};

class CollisionListener
{
public:
    CollisionListener();
    ~CollisionListener();

    const std::vector<CollisionInfo> & GetCollisionsInfo() const;
    void ClearCollisions();
    void AddCollision(const CollisionInfo & col);

private:
    std::vector<CollisionInfo> CurrentCollisions;
};
