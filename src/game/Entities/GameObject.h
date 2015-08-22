#pragma once

#include <MathLib/math.h>
#include <GraphicsEngine/SceneGraph/MeshNode.h>

#include "EntityState.h"
#include "EntityActions.h"
#include "GameObjectInventory.h"
#include "../GameEvents.h"

#include <stack>
#include <memory>

class GameObject
{
public:
    GameObject(ShiftEngine::MeshNode * sceneNode);
    virtual ~GameObject();

    // pure virtuals 
    virtual void Update(double dt) = 0;

    // setters
    virtual void SetPosition(const MathLib::Vector3F & Position);

    // getters
    virtual MathLib::Vector3F GetPosition() const;
    ShiftEngine::MeshNode * GetSceneNode();

    // highlight methods
    virtual bool CanBeHighlighted(const MathLib::Ray &uprojectedRay);
    virtual void Highlight();
    virtual void UnHightlight();

    // service methods
    bool MustBeDeleted() const;
    void Delete();

    virtual void DispatchEvent(const IGameEvent *) {}

protected:
    ShiftEngine::MeshNode * sceneNode;
    bool toDelete = false;

};

typedef std::shared_ptr<GameObject>	GameObjectPtr;
