#include "PhysicsFunctions.h"

using MathLib::Vector3D;

namespace Physics
{
	bool IsPlayerCollidesWithWorld( const Vector3D & Position, float PlayerHeight, cWorldStorage * storage )
	{
		//UNDONE: не учитывается высота игрока
		if(IsLittleCubeCollidesWithWorld(Position, 0.4f, storage) || 
			IsLittleCubeCollidesWithWorld(Vector3D(Position.x, Position.y, Position.z + 0.8f), 0.4f, storage))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool IsPointCollidesWithWorld( const Vector3D & Position, cWorldStorage * storage )
	{
		//UNDONE: не только воздух может быть неколлидящимся
		if(storage->GetBlock((int)floor(Position.x), (int)floor(Position.y), (int)floor(Position.z))->TypeID != ID_AIR)
			return true;
		return false;
	}

	bool IsLittleCubeCollidesWithWorld( const Vector3D & Pos, float Half, cWorldStorage * storage )
	{
		if(IsPointCollidesWithWorld(Vector3D(Pos.x+Half, Pos.y+Half, Pos.z), storage))		//bottom corners
			return true;
		if(IsPointCollidesWithWorld(Vector3D(Pos.x-Half, Pos.y+Half, Pos.z), storage))
			return true;
		if(IsPointCollidesWithWorld(Vector3D(Pos.x+Half, Pos.y-Half, Pos.z), storage))
			return true;
		if(IsPointCollidesWithWorld(Vector3D(Pos.x-Half, Pos.y-Half, Pos.z), storage))
			return true;
		if(IsPointCollidesWithWorld(Vector3D(Pos.x+Half, Pos.y+Half, Pos.z+2*Half), storage))	//top corners
			return true;
		if(IsPointCollidesWithWorld(Vector3D(Pos.x-Half, Pos.y+Half, Pos.z+2*Half), storage))
			return true;
		if(IsPointCollidesWithWorld(Vector3D(Pos.x+Half, Pos.y-Half, Pos.z+2*Half), storage))
			return true;
		if(IsPointCollidesWithWorld(Vector3D(Pos.x-Half, Pos.y-Half, Pos.z+2*Half), storage))
			return true;
		return false;
	}

	bool IsPointCollidesWithCube( const Vector3D & PointPosition, const Vector3D & CubePosition )
	{
		if(PointPosition.x < CubePosition.x - 0.5f)
			return false;
		if(PointPosition.x > CubePosition.x + 0.5f)
			return false;

		if(PointPosition.y < CubePosition.y - 0.5f)
			return false;
		if(PointPosition.y > CubePosition.y + 0.5f)
			return false;

		if(PointPosition.z < CubePosition.z - 0.5f)
			return false;
		if(PointPosition.z > CubePosition.z + 0.5f)
			return false;

		return true;
	}

	bool IsPlayerCollidesWithBlock( const Vector3D & PlayerPosition, const Vector3D & BlockPosition, float PlayerHeight )
	{
		if(IsPointCollidesWithCube(Vector3D(PlayerPosition.x - 0.5f, PlayerPosition.y - 0.5f, PlayerPosition.z), BlockPosition))
			return true;
		if(IsPointCollidesWithCube(Vector3D(PlayerPosition.x + 0.5f, PlayerPosition.y - 0.5f, PlayerPosition.z), BlockPosition))
			return true;
		if(IsPointCollidesWithCube(Vector3D(PlayerPosition.x - 0.5f, PlayerPosition.y + 0.5f, PlayerPosition.z), BlockPosition))
			return true;
		if(IsPointCollidesWithCube(Vector3D(PlayerPosition.x + 0.5f, PlayerPosition.y + 0.5f, PlayerPosition.z), BlockPosition))
			return true;

		if(IsPointCollidesWithCube(Vector3D(PlayerPosition.x - 0.5f, PlayerPosition.y - 0.5f, PlayerPosition.z + PlayerHeight), BlockPosition))
			return true;
		if(IsPointCollidesWithCube(Vector3D(PlayerPosition.x + 0.5f, PlayerPosition.y - 0.5f, PlayerPosition.z + PlayerHeight), BlockPosition))
			return true;
		if(IsPointCollidesWithCube(Vector3D(PlayerPosition.x - 0.5f, PlayerPosition.y + 0.5f, PlayerPosition.z + PlayerHeight), BlockPosition))
			return true;
		if(IsPointCollidesWithCube(Vector3D(PlayerPosition.x + 0.5f, PlayerPosition.y + 0.5f, PlayerPosition.z + PlayerHeight), BlockPosition))
			return true;

		return false;
	}

}