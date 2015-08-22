#pragma once

#include <MathLib/math.h>

#include <memory>

struct PhysObject
{
    PhysObject(MathLib::Vector3F _pos = MathLib::Vector3F(),
               MathLib::Vector3F _vel = MathLib::Vector3F(),
               MathLib::AABB bbox = MathLib::AABB())
               : Position(_pos)
               , Velocities(_vel)
               , bbox(bbox)
               , toDelete(false)
    {
    }

    MathLib::Vector3F Position;
    MathLib::Vector3F Velocities;
    MathLib::AABB bbox;
    bool toDelete;
};

typedef std::shared_ptr<PhysObject> PhysObjectPtr;
