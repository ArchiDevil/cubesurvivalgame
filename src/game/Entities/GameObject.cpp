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

bool GameObject::Go(const MathLib::Vector2F & target)
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
			static double rotationTime = 0.0;
			rotationTime += dt / 4.0;
			Vector3F thisPos = SceneNode->GetPosition();	thisPos.z = 0.0f;
			Vector3F target = Vector3F(targetPosition.x, targetPosition.y, 0.0);
			Vector3F faceStart = Vector3F(-1.0f, 0.0f, 0.0f) * SceneNode->GetRotation();
			Vector3F faceEnd = thisPos - target;
			auto rotation = MathLib::shortest_arc(faceStart, faceEnd);
			if(MathLib::angle(faceStart, faceEnd) <= 0.1f)
			{
				SceneNode->RotateBy(rotation);
				currentState = ES_Moving;
				rotationTime = 0.0;
			}
			else
			{
				auto rotationQuat = MathLib::quaternionSlerp(SceneNode->GetRotation(), SceneNode->GetRotation() * rotation, rotationTime);
				SceneNode->SetRotation(rotationQuat);
			}
			break;
		}
	case ES_Moving:
		{
			Vector3F currentPos = SceneNode->GetPosition();
			currentPos.z = 0.0f;
			Vector3F targetPos = Vector3F(targetPosition.x, targetPosition.y, 0.0f);
			if(MathLib::distance(currentPos, targetPos) <= 0.4f)
			{
				SceneNode->SetPosition(targetPos);
				currentState = ES_Waiting;
			}
			else
			{
				auto directionVec = MathLib::Normalize(targetPos - currentPos);
				directionVec *= dt * 5.0f;
				SceneNode->SetPosition(SceneNode->GetPosition() + directionVec);
			}
			break;
		}
	default:
		break;
	}
}
