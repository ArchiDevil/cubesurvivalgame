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

void GameObject::SetPosition( const Vector3F & Position )
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

bool GameObject::OnGameEvent(IGameEvent * ev)
{
	return false;
}

bool GameObject::Select(const MathLib::Ray & unprojectedRay)
{
	if (MathLib::RayBoxIntersect(unprojectedRay, SceneNode->GetBBox(), 0.0f, 10000.0f))
	{
		this->SceneNode->GetMaterialPtr()->SetDiffuseColor(MathLib::Vector4F(1.0f, 0.75f, 0.75f, 1.0f));
		MainLog.Message("Selected some entity");
		return true;
	}
	return false;
}

void GameObject::Unselect()
{
	this->SceneNode->GetMaterialPtr()->SetDiffuseColor(MathLib::Vector4F(1.0f, 1.0f, 1.0f, 1.0f));
	MainLog.Message("Some entity has been unselected");
}
