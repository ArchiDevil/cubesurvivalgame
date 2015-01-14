#pragma once

#include <D3D10.h>

#include <string>
#include <fstream>
#include <iostream>

#include "cMesh.h"
#include "VertexTypes.h"
#include "MiscTypes.h"
#include "Utils.h"

namespace ShiftEngine
{
	class cMeshLoader
	{
	public:
		cMeshLoader(ID3D10Device * _device);
		bool Load(std::wstring filename, MeshData * mesh);

	private:
		bool LoadFromFile(std::wstring filename, MeshData * mesh, std::unique_ptr<DefaultVertex[]> & vertices, std::unique_ptr<long[]> & indices);
		void UpdateVertices(MeshData * mesh, ShiftEngine::DefaultVertex * vertices, long * indices);

		ID3D10Device * pDevice;

	};
}
