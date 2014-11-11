#include "LivingGameObject.h"

#include "../game.h"

LivingGameObject::LivingGameObject(ShiftEngine::MeshNode * sceneNode)
	: GameObject(sceneNode)
{
}

LivingGameObject::~LivingGameObject()
{
}

bool LivingGameObject::Go(const MathLib::Vector2F & target)
{
	// check if target position doesn't contain water
	auto pGame = LostIsland::GetGamePtr();
	auto columnIndex = pGame->World->GetDataStorage()->GetColumnIndex(std::floor(target.x), std::floor(target.y));
	BlockTypes topColumn = BlockTypes::BT_Empty;

	for (unsigned i = 0;; ++i)
	{
		BlockTypes currentBlock = pGame->World->GetDataStorage()->GetBlockType(std::floor(target.x), std::floor(target.y), i++);
		if (currentBlock == BT_Empty)
			break;
		topColumn = currentBlock;
	}

	if (topColumn == BT_Water)
		return false;

	while (!states.empty() &&
		   (states.top()->GetType() == EntityState::Rotating ||
		   states.top()->GetType() == EntityState::Moving))
	{
		states.pop();
	}

	PushState(std::make_shared<MovingState>(target));
	PushState(std::make_shared<RotatingState>(target));
	//currentState = ES_Rotating;
	return true;
}

void LivingGameObject::Update(double dt)
{
#if 1
	if (!states.empty())
	{
		auto currentState = states.top();
		currentState->Update(this, dt);
		if(currentState->Dead())
		{
			states.pop();
		}
	}
#endif

#if 0
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
		if (MathLib::angle(faceStart, faceEnd) <= 0.1f)
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
		if (MathLib::distance(currentPos, targetPos) <= 0.1f)
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
#endif

	auto pGame = LostIsland::GetGamePtr();
	auto bbox = SceneNode->GetBBox();
	int heights[4] = { 0 };
	heights[0] = pGame->World->GetDataStorage()->GetFullHeight(std::floor(bbox.bMin.x), std::floor(bbox.bMin.y));
	heights[1] = pGame->World->GetDataStorage()->GetFullHeight(std::floor(bbox.bMin.x), std::floor(bbox.bMax.y));
	heights[2] = pGame->World->GetDataStorage()->GetFullHeight(std::floor(bbox.bMax.x), std::floor(bbox.bMin.y));
	heights[3] = pGame->World->GetDataStorage()->GetFullHeight(std::floor(bbox.bMax.x), std::floor(bbox.bMax.y));

	float maxHeight = heights[0];
	for (int i = 0; i < 4; ++i)
		if (maxHeight < heights[i])
			maxHeight = heights[i];

	auto position = GetPosition();
	position.z = (float)maxHeight;
	SetPosition(position);
}
