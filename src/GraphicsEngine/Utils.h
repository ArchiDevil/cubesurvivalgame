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
								unsigned int * indicesCount);	//���������� ������ � �������� �� �������� .limr �����

		MathLib::AABB getBBoxFromMesh(MeshDataPtr meshData);

		MeshDataPtr createCube();		//������� ���, �������� � ���� ���������� �������
		MeshDataPtr createPlane();		//������� ���������, �������� 1x1
		MeshDataPtr createSphere();		//������� �����, �������� � 1, � ���������� � 36
	}
}