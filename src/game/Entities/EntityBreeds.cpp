#include "EntityBreeds.h"

#include <GraphicsEngine/ShiftEngine.h>
#include <Utilities/ut.h>

#include "LiveGameObject.h"
#include "ItemGameObject.h"
#include "CollectableGameObject.h"
// #include "AnimalGameObject.h"

// Please, only clone and service methods here

ShiftEngine::MeshNode * CreateMeshNode(const std::string & meshName, const std::string & materialFile)
{
	std::wstring wmaterialFile = utils::StrToWStr(materialFile);
	auto mtl = ShiftEngine::GetContextManager()->LoadMaterial(wmaterialFile, L"generic");

	std::wstring wmeshName = utils::StrToWStr(meshName);
	MeshNode * meshNode = nullptr;
	if (wmeshName == L"cube")
		meshNode = ShiftEngine::GetSceneGraph()->AddMeshNode(ShiftEngine::Utilities::createCube(), MathLib::AABB(Vector3F(-0.5f, -0.5f, -0.5f), Vector3F(0.5f, 0.5f, 0.5f)), mtl.get());
	else
		meshNode = ShiftEngine::GetSceneGraph()->AddMeshNode(wmeshName, mtl.get());

	return meshNode;
}

GameObjectPtr LiveBreed::Clone() const
{
	return std::make_shared<LiveGameObject>(CreateMeshNode(meshName, materialFile));
}

GameObjectPtr CollectableBreed::Clone() const
{
	return std::make_shared<CollectableGameObject>(CreateMeshNode(meshName, materialFile), itemId, count);
}
