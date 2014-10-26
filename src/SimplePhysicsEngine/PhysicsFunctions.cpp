#include "PhysicsFunctions.h"

bool Physics::IsPointCollidesWithWorld(const Vector3F & Position, WorldStorage * storage)
{
	if (storage->GetBlockType((int)floor(Position.x), (int)floor(Position.y), (int)floor(Position.z)) != BlockTypes::BT_Empty)
		return true;
	return false;
}

bool Physics::IsAABBCollidesWithWorld(const MathLib::AABB & bbox, WorldStorage * storage)
{
	Vector3F corners[8];
	bbox.GetVertices(corners);
	for (int i = 0; i < 8; ++i)
		if (IsPointCollidesWithWorld(corners[i], storage))
			return true;
	return false;
}
