#pragma once

#include "vector3.h"

namespace MathLib
{
	struct Ray
	{
		Ray(const Vector3F & origin = Vector3F(0.0f, 0.0f, 0.0f), 
			const Vector3F & direction = Vector3F(0.0f, 0.0f, 0.0f))
			: Origin(origin), Direction(direction) {};

		Vector3F Origin;
		Vector3F Direction;
	};
}