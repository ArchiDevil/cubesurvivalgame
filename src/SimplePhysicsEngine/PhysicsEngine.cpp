#include "PhysicsEngine.h"

#include <string>

#include <game/world/worldStorage.h>
#include <Utilities/logger.hpp>

SimplePhysicsEngine::SimplePhysicsEngine()
    : dataStorage(nullptr)
    , gravityAcc(-9.81f)
{
}

SimplePhysicsEngine::~SimplePhysicsEngine()
{
}

void SimplePhysicsEngine::Initialize(WorldStorage * storage, float gravity)
{
    dataStorage = storage;
    gravityAcc = gravity;
}

void SimplePhysicsEngine::Update(double dt)
{
    collisionListener.ClearCollisions();
    UpdatePhysics(dt);
}

PhysObjectPtr SimplePhysicsEngine::CreateEntity(const Vector3F & position, const Vector3F & velocity, const AABB & bbox)
{
    PhysObjectPtr out(new PhysObject(position, velocity, bbox));
    physEntities.push_back(out);
    return out;
}

void SimplePhysicsEngine::UpdatePhysics(double dt)
{
    for (auto &object : physEntities)
    {
        float l = 0.0f;
        float r = (float)dt;
        float eps = 1e-6f;
        Vector3F newPos = object->Position;
        while (fabs(l - r) > eps)
        {
            float cur_dt = (l + r) / 2.0f;

            Vector3F Velocities = object->Velocities;
            Velocities.z += gravityAcc * cur_dt;
            Vector3F delta = Velocities * cur_dt;
            newPos = object->Position + delta;
            AABB bboxToCalculate = { object->bbox.bMin + newPos, object->bbox.bMax + newPos };

            if (!Physics::IsAABBCollidesWithWorld(bboxToCalculate, dataStorage))
                l = cur_dt;
            else
                r = cur_dt;
        }
        object->Velocities.z += gravityAcc * r;
        object->Position = newPos;
    }
}

const std::vector<CollisionInfo> & SimplePhysicsEngine::GetCollisions() const
{
    return collisionListener.GetCollisionsInfo();
}

int SimplePhysicsEngine::GetPhysEntitiesCount() const
{
    return physEntities.size();
}
