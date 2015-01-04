#include "GameObject.h"

#include "../game.h"

GameObject::GameObject(ShiftEngine::MeshNode * sceneNode)
	: SceneNode(sceneNode)
	, ToDelete(false)
{
}

GameObject::~GameObject()
{
	if (SceneNode)
		SceneNode->KillSelf();
}

Vector3F GameObject::GetPosition() const
{
	return SceneNode->GetPosition();
}

void GameObject::SetPosition(const Vector3F & Position)
{
	SceneNode->SetPosition(Vector3F(Position.x, Position.y, Position.z));
}

bool GameObject::MustBeDeleted() const
{
	return ToDelete;
}

void GameObject::Delete()
{
	ToDelete = true;
}

ShiftEngine::MeshNode * GameObject::GetSceneNode()
{
	return SceneNode;
}

bool GameObject::CanBeHighlighted(const MathLib::Ray &uprojectedRay)
{
	if (MathLib::RayBoxIntersect(uprojectedRay, SceneNode->GetBBox(), 0.0f, 10000.0f))
		return true;
	return false;
}

void GameObject::Highlight()
{
	SceneNode->GetMaterialPtr()->SetDiffuseColor(MathLib::Vector4F(1.5f, 1.5f, 1.5f, 1.0f));
}

void GameObject::UnHightlight()
{
	SceneNode->GetMaterialPtr()->SetDiffuseColor(MathLib::Vector4F(1.0f, 1.0f, 1.0f, 1.0f));
}
