#include "StaticGameObject.h"

StaticGameObject::StaticGameObject(ShiftEngine::MeshNode * mesh)
	: GameObject(mesh)
{
}

StaticGameObject::~StaticGameObject()
{
}

void StaticGameObject::Update(double dt)
{
	auto pGame = LostIsland::GetGamePtr();
	auto bbox = SceneNode->GetBBox();
	int heights[4] = { 0 };
	int minX = (int)std::floor(bbox.bMin.x);
	int maxX = (int)std::floor(bbox.bMax.x);
	int minY = (int)std::floor(bbox.bMin.y);
	int maxY = (int)std::floor(bbox.bMax.y);
	heights[0] = pGame->World->GetDataStorage()->GetFullHeight(minX, minY);
	heights[1] = pGame->World->GetDataStorage()->GetFullHeight(minX, maxY);
	heights[2] = pGame->World->GetDataStorage()->GetFullHeight(maxX, minY);
	heights[3] = pGame->World->GetDataStorage()->GetFullHeight(maxX, maxY);

	float maxHeight = (float)heights[0];
	for (int i = 0; i < 4; ++i)
		if (maxHeight < heights[i])
			maxHeight = heights[i];

	auto position = GetPosition();
	position.z = (float)maxHeight;
	SetPosition(position);
}
