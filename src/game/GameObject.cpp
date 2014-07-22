#include "GameObject.h"

GameObject::GameObject( ShiftEngine::MeshNode * sceneNode )
	: SceneNode(sceneNode), ToDelete(false)//, parent(nullptr)
{
}

GameObject::~GameObject()
{
	if(SceneNode)
		SceneNode->KillSelf();
}

Vector3F GameObject::GetPosition() const
{
	return SceneNode->GetPosition();
}

void GameObject::SetPosition( Vector3F & Position )
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
