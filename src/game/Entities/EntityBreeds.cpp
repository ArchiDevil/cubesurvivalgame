#include "EntityBreeds.h"

#include <GraphicsEngine/ShiftEngine.h>
#include <Utilities/ut.h>

// Please, only clone methods here

ShiftEngine::MeshNode * CreateMeshNode(const std::string & meshName, const std::string & materialFile)
{
	std::wstring wmaterialFile = utils::StrToWStr(materialFile);
	auto mtl = ShiftEngine::GetContextManager()->LoadMaterial(wmaterialFile, L"default");

	std::wstring wmeshName = utils::StrToWStr(meshName);
	MeshNode * meshNode = nullptr;
	if (wmeshName == L"cube")
		meshNode = ShiftEngine::GetSceneGraph()->AddMeshNode(ShiftEngine::Utilities::createCube(), MathLib::AABB(Vector3F(-0.5f, -0.5f, -0.5f), Vector3F(0.5f, 0.5f, 0.5f)), mtl.get());
	else
		meshNode = ShiftEngine::GetSceneGraph()->AddMeshNode(wmeshName, mtl.get());

	return meshNode;
}

GameObjectPtr ProducerBreed::Clone() const
{
	return std::make_shared<ProducerGameObject>(CreateMeshNode(meshName, materialFile), producedItem, producingTime);
}

GameObjectPtr CrafterBreed::Clone() const
{
	return std::make_shared<CrafterGameObject>(CreateMeshNode(meshName, materialFile), craftingTime);
}

GameObjectPtr StaticBreed::Clone() const
{
	return std::make_shared<StaticGameObject>(CreateMeshNode(meshName, materialFile));
}
