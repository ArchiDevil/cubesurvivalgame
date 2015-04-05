#pragma once

#include "types.h"
#include "CollisionShouter.h"
#include "PhysicsFunctions.h"

#include <Utilities/singleton.h>

#include <list>
#include <vector>

class WorldStorage;

class SimplePhysicsEngine : public singleton < SimplePhysicsEngine >
{
public:
    SimplePhysicsEngine();
    ~SimplePhysicsEngine();

    void Initialize(WorldStorage * storage, float gravity = -9.81f);
    void Update(double dt);

    PhysObjectPtr CreateEntity(const Vector3F & position, const Vector3F & velocity, const AABB & bbox);
    int GetPhysEntitiesCount() const;
    const std::vector<CollisionInfo> & GetCollisions() const;

private:
    void UpdatePhysics(double dt);

    CollisionListener collisionListener;
    WorldStorage * dataStorage;
    std::list<PhysObjectPtr> physEntities;
    float gravityAcc;

};
