#include "LivingGameObject.h"

#include "../game.h"

LivingGameObject::LivingGameObject(ShiftEngine::MeshNode * sceneNode)
	: GameObject(sceneNode)
{
}

LivingGameObject::~LivingGameObject()
{
	CancelCommands();
}

bool LivingGameObject::Go(const MathLib::Vector2F & target)
{
	// check if target position doesn't contain water
	auto pGame = LostIsland::GetGamePtr();
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

	PushCommand(std::make_unique<RotateAction>(target));
	PushCommand(std::make_unique<MoveAction>(target));
	return true;
}

void LivingGameObject::Stop()
{
	SetState(std::make_shared<WaitingState>());
}

void LivingGameObject::PushCommand(std::unique_ptr<IEntityAction> action)
{
	Actions.push(std::move(action));
}

void LivingGameObject::CancelCommands()
{
	while (!Actions.empty())
		CancelCurrentCommand();
}

void LivingGameObject::CancelCurrentCommand()
{
	if (!Actions.empty())
	{
		auto action = std::move(Actions.back());
		Actions.pop();
		action->Cancel(this);
	}
}

void LivingGameObject::Update(double dt)
{
	if (currentState->Dead())
		SetState(std::make_shared<WaitingState>());

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
