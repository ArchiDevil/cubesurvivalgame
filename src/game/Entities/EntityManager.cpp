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
{
	IProgramPtr EntityShader = GetContextManager()->LoadShader(L"EntityShader.fx");
	entityMaterial = GetContextManager()->LoadMaterial(L"entity.mtl", L"genericEntity");

	LoadEntities();
}

EntityManager::~EntityManager()
{
}

ItemEntityPtr EntityManager::CreateItemEntity(const Vector3D & Position, const Vector3D & Velocity, uint64_t itemId)
{
	auto pGame = LostIsland::GetGamePtr();
	auto * item = pGame->ItemMgr->GetItemById(itemId);

	MeshNode * meshNode = GetSceneGraph()->AddMeshNode(item->GetMesh(), MathLib::AABB(Vector3F(-0.5f, -0.5f, -0.5f), Vector3F(0.5f, 0.5f, 0.5f)), entityMaterial.get());
	std::shared_ptr<ItemEntity> out = std::make_shared<ItemEntity>(itemId, cSimplePhysicsEngine::GetInstance().CreateEntity(Position, Velocity), meshNode);
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
		(*iter)->GetSceneNode()->GetMaterialPtr()->SetNamedParam("lightPos", sunPos);

		if ((*iter)->MustBeDeleted())
			iter = GameObjects.erase(iter);
		else
			++iter;
	}
}

CrafterPtr EntityManager::CreateCrafterEntity(const Vector3F & Position, const std::string & id)
{
	auto iter = crafters.find(id);
	if (iter != crafters.end())
	{
		std::wstring mtlName = utils::StrToWStr(iter->second.materialName);
		auto mtl = ShiftEngine::GetContextManager()->LoadMaterial(mtlName, L"default");

		std::wstring meshName = utils::StrToWStr(iter->second.meshName);
		MeshNode * meshNode = nullptr;
		if (meshName == L"cube")
			meshNode = GetSceneGraph()->AddMeshNode(ShiftEngine::Utilities::createCube(), MathLib::AABB(Vector3F(-0.5f, -0.5f, -0.5f), Vector3F(0.5f, 0.5f, 0.5f)), mtl.get());
		else
			meshNode = GetSceneGraph()->AddMeshNode(meshName, mtl.get());

		auto out = std::make_shared<CrafterGameObject>(meshNode, iter->second.craftingTime);
		out->SetPosition(Position);
		GameObjects.push_back(out);
		return out;
	}

	MainLog.Error("Unable to load entity: " + id);
	return nullptr;
}

ProducerPtr EntityManager::CreateProducerEntity(const Vector3F & Position, const std::string & id)
{
	auto iter = producers.find(id);
	if (iter != producers.end())
	{
		std::wstring mtlName = utils::StrToWStr(iter->second.materialName);
		auto mtl = ShiftEngine::GetContextManager()->LoadMaterial(mtlName, L"default");

		std::wstring meshName = utils::StrToWStr(iter->second.meshName);
		MeshNode * meshNode = nullptr;
		if (meshName == L"cube")
			meshNode = GetSceneGraph()->AddMeshNode(ShiftEngine::Utilities::createCube(), MathLib::AABB(Vector3F(-0.5f, -0.5f, -0.5f), Vector3F(0.5f, 0.5f, 0.5f)), mtl.get());
		else
			meshNode = GetSceneGraph()->AddMeshNode(meshName, mtl.get());

		auto out = std::make_shared<ProducerGameObject>(meshNode, iter->second.producedItem, iter->second.producingTime);
		out->SetPosition(Position);
		GameObjects.push_back(out);
		return out;
	}

	MainLog.Error("Unable to load entity: " + id);
	return nullptr;
}

GameObjectPtr EntityManager::CreateEntity(const MathLib::Vector3F & position, const std::string & entityId)
{
	//stupid implementation now
	//for future needs, there's may be tuple with different types, needed for different entities
	auto crafters_iter = crafters.find(entityId);
	if (crafters_iter != crafters.end())
		return CreateCrafterEntity(position, entityId);

	auto producers_iter = producers.find(entityId);
	if (producers_iter != producers.end())
		return CreateProducerEntity(position, entityId);

	uint64_t itemId = LostIsland::GetGamePtr()->ItemMgr->GetItemId(entityId);
	if (itemId)
		return CreateItemEntity(position, Vector3D(), itemId);

	MainLog.Error("Unable to find entity: " + entityId);

	return nullptr;
}

PlayerPtr EntityManager::CreatePlayer(const Vector3F & Position)
{
	if (LostIsland::GetGamePtr()->Player)
	{
		MainLog.Error("Player is already created");
		return nullptr;
	}
	auto pCtxMgr = ShiftEngine::GetContextManager();
	auto pScene = ShiftEngine::GetSceneGraph();
	ShiftEngine::MaterialPtr mat = pCtxMgr->LoadMaterial(L"player.mtl", L"player");
	PlayerPtr player = std::make_shared<PlayerGameObject>(pScene->AddMeshNode(ShiftEngine::Utilities::createCube(), MathLib::AABB(Vector3F(-0.5f, -0.5f, 0.0f), Vector3F(0.5f, 0.5f, 1.0f)), mat.get()));
	GameObjects.push_back(player);
	LostIsland::GetGamePtr()->Player = player.get();
	return player;
}

void EntityManager::LoadEntities()
{
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
		if (buff.asString() == "crafter")
		{
			auto & crafter = crafters[id];

			crafter.meshName = meshName;
			crafter.materialName = materialName;

			buff = root.get("crafting_time", buff);
			crafter.craftingTime = buff.asInt();

			buff = root.get("produced_item", buff);
			crafter.craftingItem = nullptr;
		}
		else if (buff.asString() == "producer")
		{
			auto & producer = producers[id];

			producer.meshName = meshName;
			producer.materialName = materialName;

			buff = root.get("cycle_time", buff);
			producer.producingTime = buff.asInt();

			buff = root.get("produced_item", buff);
			producer.producedItem = nullptr;
		}
		else
		{
			MainLog.Error("Unable to parse " + filename + ": unknown entity type");
			continue;
		}
	}
}

void EntityManager::SelectEntity(const MathLib::Ray &unprojectedRay)
{
	bool selected = false;
	for (auto & entity : GameObjects)
	{
		if (entity->Select(unprojectedRay))
		{
			if (selectedEntity) 
				selectedEntity->Unselect();
			selectedEntity = entity.get();
			selected = true;
			break;
		}
	}

	if (!selected)
	{
		if (selectedEntity)
			selectedEntity->Unselect();
		selectedEntity = nullptr;
	}
}
