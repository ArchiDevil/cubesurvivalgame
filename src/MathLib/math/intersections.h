#pragma once

#include "ray.h"
#include "plane.h"
#include "AABB.h"
#include "OOBB.h"

namespace MathLib
{
	bool RayBoxIntersect(const Ray & ray, const AABB & bbox, float t0, float t1);
	bool SphereSphereIntersect(Vector3F s1, Vector3F s2, float r1, float r2);
	bool RaySphereIntersect(Ray & r, Vector3F & s1, float r1);
	bool LineTriangleIntersectionPoint(const Vector3F &t1, const Vector3F &t2, const Vector3F &t3, const Vector3F &l1, const Vector3F &l2, Vector3F &p);
}