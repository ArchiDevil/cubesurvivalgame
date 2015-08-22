#include "EntityBreeds.h"

#include <GraphicsEngine/ShiftEngine.h>
#include <Utilities/ut.h>

#include "LiveGameObject.h"
#include "ItemGameObject.h"
#include "CollectableGameObject.h"
#include "HeaterGameObject.h"

// Please, only clone and service methods here

ShiftEngine::MeshNode * CreateMeshNode(const std::string & meshName, const std::string & materialFile, float scale)
{
    std::wstring wmaterialFile = utils::Widen(materialFile);
    auto mtl = ShiftEngine::GetContextManager()->LoadMaterial(wmaterialFile, L"generic");

    std::wstring wmeshName = utils::Widen(meshName);
    ShiftEngine::MeshNode * meshNode = nullptr;
    if (wmeshName == L"cube")
    {
        meshNode = ShiftEngine::GetSceneGraph()->AddMeshNode(ShiftEngine::Utilities::createCube(), MathLib::AABB({ -0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f }), mtl.get());
    }
    else
    {
        meshNode = ShiftEngine::GetSceneGraph()->AddMeshNode(wmeshName, mtl.get());
    }

    if (meshNode)
    {
        meshNode->SetScale(scale);
    }

    return meshNode;
}

GameObjectPtr LiveBreed::Clone() const
{
    auto out = std::make_shared<LiveGameObject>(CreateMeshNode(meshName, materialFile, scale));
    for (const auto &item : inventoryBreedPtr.GetItems())
        out->GetInventory()->AddItem(item.itemId, item.count ? item.count : item.minimumCount + (rand() % (item.maximumCount - item.minimumCount)));
    return out;
}

GameObjectPtr CollectableBreed::Clone() const
{
    return std::make_shared<CollectableGameObject>(CreateMeshNode(meshName, materialFile, scale), itemId, count);
}

GameObjectPtr HeaterBreed::Clone() const
{
    return std::make_shared<HeaterGameObject>(CreateMeshNode(meshName, materialFile, scale), heatCount);
}
