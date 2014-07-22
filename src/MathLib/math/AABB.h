#pragma once

#include <cmath>
#include <algorithm>

#include "vector3.h"

namespace MathLib
{
	class AABB
	{
	public:
		AABB(const Vector3F & _bmin = Vector3F(0.0f, 0.0f, 0.0f), 
			 const Vector3F & _bmax = Vector3F(0.0f, 0.0f, 0.0f));

		void GetVertices(Vector3F vertices[8]) const;

		bool IntersectsAABB(const AABB & otherAABBox) const;
		bool IntersectsLineSegment(const Vector3F &mid, const Vector3F &dir, const int hl) const;

		void Include(const AABB & otherAABBox);
		float GetSurfaceArea() const;
		float GetVolume() const;

		Vector3F bMin;
		Vector3F bMax;
	};
}
