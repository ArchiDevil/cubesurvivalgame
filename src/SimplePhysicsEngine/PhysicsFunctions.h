#pragma once

#include <MathLib/math.h>
#include <game/world/cWorldStorage.h>

using MathLib::Vector3D;

namespace Physics
{
	bool IsPointCollidesWithWorld (const Vector3D & Position, cWorldStorage * world);							//проверяет коллизию точки и мира
	bool IsLittleCubeCollidesWithWorld (const Vector3D & Pos, float Half, cWorldStorage * storage);				//проверяет коллизию маленького куба и мира
	bool IsPlayerCollidesWithWorld (const Vector3D & Position, float PlayerHeight, cWorldStorage * storage);	//проверяет коллизию игрока и мира
	bool IsPointCollidesWithCube (const Vector3D & PointPosition, const Vector3D & CubePosition);				//проверяет коллизию точки и куба (1,1,1)
	bool IsPlayerCollidesWithBlock (const Vector3D & PlayerPosition, const Vector3D & BlockPosition, float PlayerHeight);	//проверяет коллизию игрока и блока
}
