#include "GameObject.h"

#include "../game.h"

GameObject::GameObject(ShiftEngine::MeshNode * sceneNode)
    : sceneNode(sceneNode)
{
}

GameObject::~GameObject()
{
    if (sceneNode)
        sceneNode->KillSelf();
}

Vector3F GameObject::GetPosition() const
{
    return sceneNode->GetPosition();
}

void GameObject::SetPosition(const Vector3F & Position)
{
    sceneNode->SetPosition(Vector3F(Position.x, Position.y, Position.z));
}

bool GameObject::MustBeDeleted() const
{
    return toDelete;
}

void GameObject::Delete()
{
    toDelete = true;
}

ShiftEngine::MeshNode * GameObject::GetSceneNode()
{
    return sceneNode;
}

bool GameObject::CanBeHighlighted(const MathLib::Ray &uprojectedRay)
{
    if (MathLib::RayBoxIntersect(uprojectedRay, sceneNode->GetBBox(), 0.0f, 10000.0f))
        return true;
    return false;
}

void GameObject::Highlight()
{
    sceneNode->GetMaterialPtr()->SetDiffuseColor(MathLib::Vector4F(1.5f, 1.5f, 1.5f, 1.0f));
}

void GameObject::UnHightlight()
{
    sceneNode->GetMaterialPtr()->SetDiffuseColor(MathLib::Vector4F(1.0f, 1.0f, 1.0f, 1.0f));
}
