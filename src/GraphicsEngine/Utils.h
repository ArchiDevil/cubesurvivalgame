#pragma once

#include "VertexTypes.h"
#include "MiscTypes.h"
#include "cMesh.h"

namespace ShiftEngine
{
	namespace Utilities
	{
		bool getVerticesFromFile(const std::wstring & filename,
								SerializedLIM & vertices,
								std::vector<unsigned long> & indices);

		MathLib::AABB getBBoxFromMesh(MeshDataPtr meshData);

		MeshDataPtr createCube();
		MeshDataPtr createPlane();
		MeshDataPtr createSphere();
	}
}
