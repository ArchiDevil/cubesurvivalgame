#pragma once

#include "VertexTypes.h"
#include "MiscTypes.h"
#include "cMesh.h"

namespace ShiftEngine
{
	namespace Utilities
	{
		bool getVerticesFromFile(const std::wstring & filename,
								std::unique_ptr<DefaultVertex[]>& vertices,
								unsigned int * verticesCount,
								std::unique_ptr<long[]>& indices,
								unsigned int * indicesCount);

		MathLib::AABB getBBoxFromMesh(MeshDataPtr meshData);

		MeshDataPtr createCube();
		MeshDataPtr createPlane();
		MeshDataPtr createSphere();
	}
}