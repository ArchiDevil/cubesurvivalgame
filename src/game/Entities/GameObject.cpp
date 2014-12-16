#include "GameObject.h"

#include "../game.h"

GameObject::GameObject(ShiftEngine::MeshNode * sceneNode)
	: SceneNode(sceneNode)
	, currentState(std::make_unique<WaitingState>())
	, ToDelete(false)
	, health(1)
	, inventory(LostIsland::GetGamePtr()->ItemMgr, 10)
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

int GameObject::GetHealth() const
{
	return health;
}

void GameObject::SetHealth(int in_health)
{
	health = in_health;
}

bool GameObject::DispatchState(std::unique_ptr<IEntityState> state)
{
	if (currentState && currentState->DispatchState(state->GetType()))
	{
		EntityState fromState = currentState->GetType();
		EntityState toState = state->GetType();
		currentState = std::move(state);
		OnStateChange(fromState, toState);
		return true;
	}
	return false;
}

const EntityState GameObject::GetCurrentState() const
{
	return currentState->GetType();
}

void GameObject::Highlight()
{
	SceneNode->GetMaterialPtr()->SetDiffuseColor(MathLib::Vector4F(1.5f, 1.5f, 1.5f, 1.0f));
}

void GameObject::UnHightlight()
{
	SceneNode->GetMaterialPtr()->SetDiffuseColor(MathLib::Vector4F(1.0f, 1.0f, 1.0f, 1.0f));
}

std::unique_ptr<IEntityAction> GameObject::GetInteraction()
{
	return nullptr;
}

void GameObject::OnStateChange(EntityState /*from*/, EntityState /*to*/)
{
}
