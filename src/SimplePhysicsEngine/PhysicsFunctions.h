#pragma once

#include <MathLib/math.h>
#include <game/world/worldStorage.h>

namespace Physics
{
    bool IsPointCollidesWithWorld(const MathLib::Vector3F & Position, WorldStorage * world);
    bool IsAABBCollidesWithWorld(const MathLib::AABB & bbox, WorldStorage * storage);
}
