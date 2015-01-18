#pragma once

#include <D3D10.h>

#include <string>
#include <set>

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
		bool Load(const std::wstring &filename, MeshData * mesh);

	private:
		void UpdateVertices(SerializedLIM & vertices, std::vector<unsigned long> & indices) const;
		const VertexSemantic * CreateSemantic(const SerializedLIM & vertices);

		ID3D10Device * pDevice;
		std::set<VertexSemantic> semantics;

	};
}
