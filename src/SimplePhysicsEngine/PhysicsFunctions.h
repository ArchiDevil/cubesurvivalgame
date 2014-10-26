#pragma once

#include <MathLib/math.h>
#include <game/world/worldStorage.h>

using MathLib::Vector3F;

namespace Physics
{
	bool IsPointCollidesWithWorld(const Vector3F & Position, WorldStorage * world);
	bool IsAABBCollidesWithWorld(const MathLib::AABB & bbox, WorldStorage * storage);
}
