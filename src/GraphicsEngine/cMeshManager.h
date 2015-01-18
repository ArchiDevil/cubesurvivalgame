#pragma once

#include <D3D10.h>

#include <string>
#include <map>

#include "cMeshLoader.h"
#include "cMesh.h"

namespace ShiftEngine
{
	class cMeshManager
	{
	public:
		cMeshManager(ID3D10Device * _device);
		~cMeshManager();

		void		Initialize();
		MeshDataPtr LoadMesh(const std::wstring & fileName);
		void		AddDirectly(MeshDataPtr meshPtr, std::wstring name);
		MeshDataPtr CreateMeshFromVertices(const void * verticesData, size_t verticesCount,
										   const void * indexData, size_t indicesCount,
										   const ShiftEngine::VertexSemantic * semantic,
										   const D3D10VDPtr declaration);

	private:
		MeshDataPtr ErrorMesh;
		cMeshLoader * MeshLoader;
		std::wstring ErrorName;
		std::map<std::wstring, MeshDataPtr> MeshesData;
		ID3D10Device * device;

	};
}
