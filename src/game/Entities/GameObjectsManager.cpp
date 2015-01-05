#include "GameObjectsManager.h"

#include <vector>
#include <algorithm>

#include <GraphicsEngine/ShiftEngine.h>
#include <GraphicsEngine/D3D10ContextManager.h>
#include <GraphicsEngine/SceneGraph/SceneGraph.h>

#include <Utilities/ut.h>

#include "../game.h"

using namespace ShiftEngine;

GameObjectsManager::GameObjectsManager()
	: selectedEntity(nullptr)
	, entityMaterial(nullptr)
{
	IProgramPtr EntityShader = GetContextManager()->LoadShader(L"EntityShader.fx");
	entityMaterial = GetContextManager()->LoadMaterial(L"entity.mtl", L"genericEntity");
}

GameObjectsManager::~GameObjectsManager()
{
}

ItemGameObjectPtr GameObjectsManager::CreateItemEntity(const Vector3F & Position, const Vector3F & Velocity, uint64_t itemId)
{
	auto pGame = LostIsland::GetGamePtr();
	auto * item = pGame->ItemMgr->GetItemById(itemId);

	float scale = 0.4f;
	MathLib::AABB bbox = { Vector3F(-0.5f, -0.5f, -0.5f), Vector3F(0.5f, 0.5f, 0.5f) };
	bbox.bMin *= scale;
	bbox.bMax *= scale;
	MeshNode * meshNode = GetSceneGraph()->AddMeshNode(item->GetMesh(), bbox, entityMaterial.get());
	std::shared_ptr<ItemGameObject> out = std::make_shared<ItemGameObject>(itemId, SimplePhysicsEngine::GetInstance().CreateEntity(Position, Velocity, bbox), meshNode);
	meshNode->GetMaterialPtr()->SetDiffuseTexture(item->GetTexturePtr());
	meshNode->SetScale(Vector3F(scale, scale, scale));
	GameObjects.push_back(out);
	return out;
}

void GameObjectsManager::Update( double dt )
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

GameObjectPtr GameObjectsManager::CreateEntity(const MathLib::Vector3F & position, const std::string & entityId)
{
	auto iter = Breeds.find(entityId);
	if (iter == Breeds.end())
	{
		LOG_ERROR("Unable to create entity with id: " + entityId);
		return nullptr;
	}

	auto out = iter->second->Clone();
	out->SetPosition(position);
	GameObjects.push_back(out);
	return out;
}

PlayerPtr GameObjectsManager::CreatePlayer(const Vector3F & Position)
{
	auto pGame = LostIsland::GetGamePtr();
	if (pGame->Player)
	{
		LOG_ERROR("Player is already created");
		return nullptr;
	}
	auto pCtxMgr = ShiftEngine::GetContextManager();
	auto pScene = ShiftEngine::GetSceneGraph();

	ShiftEngine::MaterialPtr mat = pCtxMgr->LoadMaterial(L"player.mtl", L"player");
	PlayerPtr player = std::make_shared<PlayerGameObject>(pScene->AddMeshNode(ShiftEngine::Utilities::createCube(), MathLib::AABB(Vector3F(-0.5f, -0.5f, 0.0f), Vector3F(0.5f, 0.5f, 1.0f)), mat.get()), pGame->ItemMgr.get());
	player->SetPosition(Position);
	GameObjects.push_back(player);
	pGame->Player = player.get();
	return player;
}

void GameObjectsManager::HighlightEntity(const MathLib::Ray &unprojectedRay)
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

GameObjectPtr GameObjectsManager::GetNearestEntity(const MathLib::Ray &unprojectedRay)
{
	for (auto & entity : GameObjects)
		if (entity->CanBeHighlighted(unprojectedRay))
			return entity;
	return nullptr;
}

void GameObjectsManager::LoadEntities()
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
			LOG_ERROR("Unable to open " + filename);
			continue;
		}

		Json::Value root(0);
		bool result = reader.parse(jsonfile, root);
		if (!result)
		{
			LOG_ERROR("Unable to parse " + filename);
			LOG_ERROR(reader.getFormatedErrorMessages());
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
				LOG_ERROR("Wrong entity " + filename);
				LOG_ERROR("Unable to find property " + prop);
				errorFlag = true;
			}
		}
		if (errorFlag) continue;

		std::string id = root.get("id", buff).asString();
		std::string meshName = root.get("mesh", buff).asString();
		std::string materialName = root.get("material", buff).asString();

		buff = root.get("type", buff);
		std::string type = buff.asString();
		if (type == "static")
		{
			Breeds[id] = std::make_shared<LiveBreed>(meshName, materialName);
		}
		else if (type == "collectable")
		{
			buff = root.get("item", buff);
			std::string itemName = buff.asString();
			if (itemName.empty())
			{
				LOG_ERROR("Unable to find entity item for: " + id);
				continue;
			}
			item_id_t itemId = pGame->ItemMgr->GetItemId(itemName);
			buff = root.get("count", buff);
			unsigned count = buff.asUInt();
			Breeds[id] = std::make_shared<CollectableBreed>(meshName, materialName, itemId, count);
		}
		else
		{
			LOG_ERROR("Unable to parse " + filename + ": unknown entity type");
			continue;
		}
	}
}
