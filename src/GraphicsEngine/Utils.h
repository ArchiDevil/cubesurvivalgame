#pragma once

#include "VertexTypes.h"
#include "MiscTypes.h"
#include "cMesh.h"

namespace ShiftEngine
{
	namespace Utilities
	{
		bool getVerticesFromFile(const std::wstring & filename, 
								ShiftEngine::DefaultVertex ** vertices, 
								unsigned int * verticesCount, 
								long ** indices, 
								unsigned int * indicesCount);	//возвращает данные о вершинах из целевого .limr файла

		MathLib::AABB getBBoxFromMesh(MeshDataPtr meshData);

		MeshDataPtr createCube();		//создает куб, размером в одну кубическую единицу
		MeshDataPtr createPlane();		//создает плоскость, размером 1x1
		MeshDataPtr createSphere();		//создает сферу, радиусом в 1, с разбиением в 36
	}
}