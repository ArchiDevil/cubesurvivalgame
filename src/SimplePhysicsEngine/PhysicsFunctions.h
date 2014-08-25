#pragma once

#include <MathLib/math.h>
#include <game/world/cWorldStorage.h>

using MathLib::Vector3D;

namespace Physics
{
	bool IsPointCollidesWithWorld (const Vector3D & Position, cWorldStorage * world);							//��������� �������� ����� � ����
	bool IsLittleCubeCollidesWithWorld (const Vector3D & Pos, float Half, cWorldStorage * storage);				//��������� �������� ���������� ���� � ����
	bool IsPlayerCollidesWithWorld (const Vector3D & Position, float PlayerHeight, cWorldStorage * storage);	//��������� �������� ������ � ����
	bool IsPointCollidesWithCube (const Vector3D & PointPosition, const Vector3D & CubePosition);				//��������� �������� ����� � ���� (1,1,1)
	bool IsPlayerCollidesWithBlock (const Vector3D & PlayerPosition, const Vector3D & BlockPosition, float PlayerHeight);	//��������� �������� ������ � �����
}
