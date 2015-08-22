#include "PhysicsFunctions.h"

bool Physics::IsPointCollidesWithWorld(const MathLib::Vector3F & Position, WorldStorage * storage)
{
    if (!storage)
        return false;

    if (storage->GetBlockType((int)floor(Position.x), (int)floor(Position.y), (int)floor(Position.z)) != BlockTypes::BT_Empty)
        return true;

    return false;
}

bool Physics::IsAABBCollidesWithWorld(const MathLib::AABB & bbox, WorldStorage * storage)
{
    if (!storage)
        return false;

    MathLib::Vector3F corners[8];
    bbox.GetVertices(corners);
    for (int i = 0; i < 8; ++i)
        if (corners[i].z < 0.0f || IsPointCollidesWithWorld(corners[i], storage))
            return true;
    return false;
}
