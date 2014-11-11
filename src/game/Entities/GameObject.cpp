#include "GameObject.h"

GameObject::GameObject( ShiftEngine::MeshNode * sceneNode )
	: SceneNode(sceneNode)
	, ToDelete(false)
	, health(1)
	//, parent(nullptr)
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

void GameObject::Select()
{
	this->SceneNode->GetMaterialPtr()->SetDiffuseColor(MathLib::Vector4F(1.0f, 0.75f, 0.75f, 1.0f));
	MainLog.Message("Selected some entity");
}

void GameObject::Unselect()
{
	this->SceneNode->GetMaterialPtr()->SetDiffuseColor(MathLib::Vector4F(1.0f, 1.0f, 1.0f, 1.0f));
	MainLog.Message("Some entity has been unselected");
}

bool GameObject::CanSelected(const MathLib::Ray &uprojectedRay)
{
	if (MathLib::RayBoxIntersect(uprojectedRay, SceneNode->GetBBox(), 0.0f, 10000.0f))
		return true;
	return false;
}

uint32_t GameObject::GetHealth() const
{
	return health;
}

void GameObject::SetHealth(uint32_t health)
{
	this->health = health;
}

void GameObject::PushState(const std::shared_ptr<IEntityState> & state)
{
	states.push(state);
}

const EntityState GameObject::GetCurrentState() const
{
	if (states.empty())
		return EntityState::Waiting;
	return states.top()->GetType();
}
