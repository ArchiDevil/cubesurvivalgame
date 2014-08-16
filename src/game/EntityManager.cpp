#include "EntityManager.h"

#include <vector>
#include <algorithm>

#include "../GraphicsEngine/D3D10ContextManager.h"
#include "../GraphicsEngine/SceneGraph/SceneGraph.h"

using namespace ShiftEngine;

cEntityManager::cEntityManager()
{
}

cEntityManager::~cEntityManager()
{
}

void cEntityManager::Initialize()
{
	IProgramPtr EntityShader = GetContextManager()->LoadShader(L"EntityShader.fx");
	entityMaterial = GetContextManager()->LoadMaterial(L"entity.mtl", L"genericEntity");
}

ItemEntityPtr cEntityManager::CreateItemEntity( Vector3D Position, Vector3D Velocity, Item * item )
{
	MeshNode * meshNode = GetSceneGraph()->AddMeshNode(*item->GetMesh(), MathLib::AABB(Vector3F(-0.5f, -0.5f, -0.5f), Vector3F(0.5f, 0.5f, 0.5f)), entityMaterial.get());
	ItemEntity * it = new ItemEntity(item, cSimplePhysicsEngine::GetInstance().CreateEntity(Position, Velocity), meshNode);
	meshNode->GetMaterialPtr()->SetDiffuseTexture(it->GetItemPtr()->GetTexturePtr());
	meshNode->SetScale(Vector3F(0.4f, 0.4f, 0.4f));
	std::shared_ptr<ItemEntity> t(it);
	GameObjects.push_back(t);
	return t;
}

void cEntityManager::Update( double dt, const Vector3F & sunPos )
{
	for (auto iter = GameObjects.begin(); iter != GameObjects.end(); iter++)
	{
		if((*iter)->MustBeDeleted())
		{
			iter = GameObjects.erase(iter);
			if(iter == GameObjects.end())
				return;
		}

		(*iter)->Update(dt);
		(*iter)->GetSceneNode()->GetMaterialPtr()->SetNamedParam("lightPos", sunPos);
	}
}

GameObjectPtr cEntityManager::FindEntityByPos( Vector3F Position )
{
	for (auto iter = GameObjects.begin(); iter != GameObjects.end(); iter++)
	{
		if((*iter)->GetPosition() == Position && !(*iter)->MustBeDeleted())
		{
			return (*iter);
		}
	}
	return nullptr;
}

vector<ItemEntity *> cEntityManager::FindItemsNearPlayer( Vector3F playerPos )
{
	vector<ItemEntity*> out;
	for (auto iter = GameObjects.begin(); iter != GameObjects.end(); iter++)
	{
		if(MathLib::distance((*iter)->GetPosition(), playerPos) < 2.0f)
		{
			ItemEntity * ptr = dynamic_cast<ItemEntity*>(&*(*iter));
			if(ptr) 
				out.push_back(ptr);
		}
	}
	return out;
}
