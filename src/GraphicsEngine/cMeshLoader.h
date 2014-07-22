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

		bool Load(std::wstring filename, MeshData * mesh); //загрузка из файла

	private:
		bool LoadFromFile(std::wstring filename, 
			MeshData * mesh, 
			ShiftEngine::DefaultVertex ** vertices, 
			long ** indices);			//выполняет загрузку меша из файла

		void UpdateVertices(MeshData * mesh, 
			ShiftEngine::DefaultVertex * vertices, 
			long * indices);		//"обновляет" вершины и индексы для использования в принятой СК

		ID3D10Device * pDevice;

	};
}
