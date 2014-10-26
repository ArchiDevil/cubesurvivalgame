#pragma once

#include <memory>

#include <MathLib/math.h>

using MathLib::Vector3F;

struct PhysObject
{
	PhysObject(Vector3F _pos = Vector3F(0.0f, 0.0f, 0.0f),
			   Vector3F _vel = Vector3F(0.0f, 0.0f, 0.0f))
		: Position(_pos)
		, Velocities(_vel)
		, toDelete(false)
	{
	}

	Vector3F Position;
	Vector3F Velocities;
	bool toDelete;
};

typedef std::shared_ptr<PhysObject> PhysObjectPtr;
