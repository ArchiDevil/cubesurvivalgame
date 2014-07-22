#include "cMeshManager.h"

ShiftEngine::cMeshManager::cMeshManager( ID3D10Device * _device )
	: device(_device)
{
	MeshLoader = new cMeshLoader(_device);
}

ShiftEngine::cMeshManager::~cMeshManager()
{
	delete MeshLoader;
}

ShiftEngine::MeshDataPtr ShiftEngine::cMeshManager::LoadMesh( const std::wstring & fileName )
{
	auto iter = MeshesData.find(fileName);
	if(iter != MeshesData.end())
	{
		return iter->second;
	}
	else
	{
		MeshData * ptr = new MeshData;
		if(!MeshLoader->Load(fileName, ptr))
		{
			delete ptr;
			return nullptr;
		}
		else
		{
			MeshesData[fileName] = MeshDataPtr(ptr);
			return MeshDataPtr(ptr);
		}
	}
}

ShiftEngine::MeshDataPtr ShiftEngine::cMeshManager::CreateMeshFromVertices(const void * data, size_t count, 
												const void * indexData, size_t indicesCount,
												const ShiftEngine::VertexSemantic * semantic,
												const D3D10VDPtr declaration)
{
	MeshDataPtr out = std::make_shared<MeshData>(declaration);
	if (!out->CreateBuffers(false, data, count, indexData, indicesCount, device))
		throw;
	out->vertexSemantic = semantic;
	return out;
}

void ShiftEngine::cMeshManager::AddDirectly( MeshDataPtr meshPtr, std::wstring name )
{
	MeshesData[name] = meshPtr;
}
