#include "LivingGameObject.h"

#include "../game.h"

LivingGameObject::LivingGameObject(ShiftEngine::MeshNode * sceneNode)
	: GameObject(sceneNode)
{
}

LivingGameObject::~LivingGameObject()
{
	while (!Actions.empty())
	{
		auto action = Actions.front();
		action->Cancel(this);
		Actions.pop();
	}
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

	PushState(std::make_shared<MovingState>(target));
	PushState(std::make_shared<RotatingState>(target));

	Actions.push(std::make_shared<RotateAction>(target));
	Actions.push(std::make_shared<MoveAction>(target));

	return true;
}

void LivingGameObject::Stop()
{
	// set current state to waiting
	while (!states.empty() &&
		   states.top()->GetType() != EntityState::Waiting)
	{
		states.pop();
	}
}

void LivingGameObject::Update(double dt)
{
	auto currentState = states.top();
	currentState->Update(this, dt);
	if (currentState->Dead())
		states.pop();

	if (!Actions.empty())
	{
		auto & action = Actions.front();
		action->Update(this, dt);
		if (action->IsDead())
			Actions.pop();
	}

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
