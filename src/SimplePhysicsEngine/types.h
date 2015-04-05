#pragma once

#include <MathLib/math.h>

#include <memory>

using MathLib::Vector3F;
using MathLib::AABB;

struct PhysObject
{
    PhysObject(Vector3F _pos = Vector3F(),
               Vector3F _vel = Vector3F(),
               AABB bbox = AABB())
               : Position(_pos)
               , Velocities(_vel)
               , bbox(bbox)
               , toDelete(false)
    {
    }

    Vector3F Position;
    Vector3F Velocities;
    AABB bbox;
    bool toDelete;
};

typedef std::shared_ptr<PhysObject> PhysObjectPtr;
