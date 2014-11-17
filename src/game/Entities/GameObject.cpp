#include "GameObject.h"

GameObject::GameObject( ShiftEngine::MeshNode * sceneNode )
	: SceneNode(sceneNode)
	, ToDelete(false)
	, health(1)
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
	MainLog.Message("Entity " + std::to_string((size_t)this) + " died");
}

ShiftEngine::MeshNode * GameObject::GetSceneNode()
{
	return SceneNode;
}

bool GameObject::OnGameEvent(IGameEvent * ev)
{
	return false;
}

bool GameObject::CanBeHighlighted(const MathLib::Ray &uprojectedRay)
{
	if (MathLib::RayBoxIntersect(uprojectedRay, SceneNode->GetBBox(), 0.0f, 10000.0f))
		return true;
	return false;
}

int GameObject::GetHealth() const
{
	return health;
}

void GameObject::SetHealth(int in_health)
{
	health = in_health;
	if (health <= 0)
	{
		PushState(std::make_shared<DecayState>(10.0f));
		PushState(std::make_shared<DyingState>(2.0f));
	}
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

void GameObject::Highlight()
{
	SceneNode->GetMaterialPtr()->SetDiffuseColor(MathLib::Vector4F(1.5f, 1.5f, 1.5f, 1.0f));
}

void GameObject::UnHightlight()
{
	SceneNode->GetMaterialPtr()->SetDiffuseColor(MathLib::Vector4F(1.0f, 1.0f, 1.0f, 1.0f));
}
