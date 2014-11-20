#include "EntityManager.h"

#include <vector>
#include <algorithm>

#include <GraphicsEngine/ShiftEngine.h>
#include <GraphicsEngine/D3D10ContextManager.h>
#include <GraphicsEngine/SceneGraph/SceneGraph.h>

#include <Utilities/ut.h>

#include "../game.h"

using namespace ShiftEngine;

EntityManager::EntityManager()
	: selectedEntity(nullptr)
	, entityMaterial(nullptr)
{
	IProgramPtr EntityShader = GetContextManager()->LoadShader(L"EntityShader.fx");
	entityMaterial = GetContextManager()->LoadMaterial(L"entity.mtl", L"genericEntity");
}

EntityManager::~EntityManager()
{
}

ItemGameObjectPtr EntityManager::CreateItemEntity(const Vector3F & Position, const Vector3F & Velocity, uint64_t itemId)
{
	auto pGame = LostIsland::GetGamePtr();
	auto * item = pGame->ItemMgr->GetItemById(itemId);

	MeshNode * meshNode = GetSceneGraph()->AddMeshNode(item->GetMesh(), MathLib::AABB(Vector3F(-0.5f, -0.5f, -0.5f), Vector3F(0.5f, 0.5f, 0.5f)), entityMaterial.get());
	std::shared_ptr<ItemGameObject> out = std::make_shared<ItemGameObject>(itemId, cSimplePhysicsEngine::GetInstance().CreateEntity(Position, Velocity), meshNode);
	meshNode->GetMaterialPtr()->SetDiffuseTexture(item->GetTexturePtr());
	meshNode->SetScale(Vector3F(0.4f, 0.4f, 0.4f));
	GameObjects.push_back(out);
	return out;
}

void EntityManager::Update( double dt, const Vector3F & sunPos )
{
	auto iter = GameObjects.begin();
	while (iter != GameObjects.end())
	{
		(*iter)->Update(dt);
		if ((*iter)->MustBeDeleted())
			iter = GameObjects.erase(iter);
		else
			++iter;
	}
}

GameObjectPtr EntityManager::CreateEntity(const MathLib::Vector3F & position, const std::string & entityId)
{
	auto iter = Breeds.find(entityId);
	if (iter == Breeds.end())
	{
		MainLog.Error("Unable to create entity with id: " + entityId);
		return nullptr;
	}

	auto out = iter->second->Clone();
	out->SetPosition(position);
	GameObjects.push_back(out);
	return out;
}

PlayerPtr EntityManager::CreatePlayer(const Vector3F & Position)
{
	auto pGame = LostIsland::GetGamePtr();
	if (pGame->Player)
	{
		MainLog.Error("Player is already created");
		return nullptr;
	}
	auto pCtxMgr = ShiftEngine::GetContextManager();
	auto pScene = ShiftEngine::GetSceneGraph();

	ShiftEngine::MaterialPtr mat = pCtxMgr->LoadMaterial(L"player.mtl", L"player");
	PlayerPtr player = std::make_shared<PlayerGameObject>(pScene->AddMeshNode(ShiftEngine::Utilities::createCube(), MathLib::AABB(Vector3F(-0.5f, -0.5f, 0.0f), Vector3F(0.5f, 0.5f, 1.0f)), mat.get()), pGame->ItemMgr);
	GameObjects.push_back(player);
	pGame->Player = player.get();
	return player;
}

void EntityManager::HighlightEntity(const MathLib::Ray &unprojectedRay)
{
	if (selectedEntity && selectedEntity->CanBeHighlighted(unprojectedRay))
		return;

	bool selected = false;
	for (auto & entity : GameObjects)
	{
		if (entity->CanBeHighlighted(unprojectedRay))
		{
			entity->Highlight();
			if (selectedEntity) 
				selectedEntity->UnHightlight();
			selectedEntity = entity.get();
			selected = true;
			break;
		}
	}

	if (!selected)
	{
		if (selectedEntity)
			selectedEntity->UnHightlight();
		selectedEntity = nullptr;
	}
}

GameObjectPtr EntityManager::GetNearestEntity(const MathLib::Ray &unprojectedRay)
{
	for (auto & entity : GameObjects)
		if (entity->CanBeHighlighted(unprojectedRay))
			return entity;
	return nullptr;
}

void EntityManager::LoadEntities()
{
	auto *pGame = LostIsland::GetGamePtr();

	std::wstring pathPrefix = L"resources/gamedata/entities/";
	auto files = utils::filesystem::CollectFileNames(pathPrefix);

	Json::Reader reader;
	for (auto & file : files)
	{
		file = pathPrefix + file;
		std::string filename = utils::WStrToStr(file);
		std::ifstream jsonfile(filename);
		if (jsonfile.fail() || jsonfile.bad())
		{
			MainLog.Error("Unable to open " + filename);
			continue;
		}

		Json::Value root(0);
		bool result = reader.parse(jsonfile, root);
		if (!result)
		{
			MainLog.Error("Unable to parse " + filename);
			MainLog.Error(reader.getFormatedErrorMessages());
			continue;
		}

		//check common values
		std::string props[] = {
			"id", "mesh", "material", "type"
		};

		Json::Value buff(0);
		bool errorFlag = false;
		for (auto & prop : props)
		{
			buff = root.get(prop, buff);
			if (buff.empty())
			{
				MainLog.Error("Wrong entity " + filename);
				MainLog.Error("Unable to find property " + prop);
				errorFlag = true;
			}
		}
		if (errorFlag) continue;

		std::string id = root.get("id", buff).asString();
		std::string meshName = root.get("mesh", buff).asString();
		std::string materialName = root.get("material", buff).asString();

		buff = root.get("type", buff);
		std::string type = buff.asString();
		if (type == "crafter")
		{
			buff = root.get("crafting_time", buff);
			uint32_t craftingTime = buff.asInt();
			buff = root.get("produced_item", buff);
			Item * craftingItem = nullptr;
			//UNDONE: crafting item
			Breeds[id] = std::make_shared<CrafterBreed>(meshName, materialName, craftingItem, craftingTime);
		}
		else if (type == "static")
		{
			Breeds[id] = std::make_shared<StaticBreed>(meshName, materialName);
		}
		else if (type == "collectable")
		{
			buff = root.get("item", buff);
			std::string itemName = buff.asString();
			if (itemName.empty())
			{
				MainLog.Error("Unable to find entity item for: " + id);
				continue;
			}
			uint32_t itemId = pGame->ItemMgr->GetItemId(itemName);
			Breeds[id] = std::make_shared<CollectableBreed>(meshName, materialName, itemId);
		}
		else
		{
			MainLog.Error("Unable to parse " + filename + ": unknown entity type");
			continue;
		}
	}
}
