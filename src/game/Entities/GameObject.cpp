#include "GameObject.h"

GameObject::GameObject( ShiftEngine::MeshNode * sceneNode )
	: SceneNode(sceneNode)
	, ToDelete(false)
	, targetRotationAngle(0.0f)
	, currentState(ES_Waiting)
	, targetPosition()
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

bool GameObject::Go(const MathLib::Vector3F & target)
{
	targetPosition = target;
	currentState = ES_Rotating;
	return true;
}

void GameObject::Update(double dt)
{
	switch (currentState)
	{
	case ES_Rotating:
		{
			Vector3F start(1.0f, 0.0f, 0.0f);
			auto rotQuat = MathLib::shortest_arc(start, SceneNode->GetPosition() - targetPosition);
			SceneNode->SetRotation(rotQuat.to_vector());
			currentState = ES_Moving;
			break;
		}
	case ES_Moving:
		{
			if(MathLib::distance(SceneNode->GetPosition(), targetPosition) <= 1.0f)
			{
				SceneNode->SetPosition(targetPosition);
				currentState = ES_Waiting;
			}
			auto directionVec = MathLib::Normalize(targetPosition - SceneNode->GetPosition());
			directionVec *= dt * 5.0f;
			SceneNode->SetPosition(SceneNode->GetPosition() + directionVec);
			break;
		}
	default:
		break;
	}
}
