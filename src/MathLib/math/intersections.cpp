#include "intersections.h"

#include "../math.h"

bool MathLib::RayBoxIntersect( const Ray & ray, const AABB & bbox, float t0, float t1 )
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	float divx = 1.0f / ray.Direction.x;
	if (divx >= 0.0f)
	{
		tmin = (bbox.bMin.x - ray.Origin.x) * divx;
		tmax = (bbox.bMax.x - ray.Origin.x) * divx;
	}
	else
	{
		tmin = (bbox.bMax.x - ray.Origin.x) * divx;
		tmax = (bbox.bMin.x - ray.Origin.x) * divx;
	}

	float divy = 1.0f / ray.Direction.y;
	if (divy >= 0.0f)
	{
		tymin = (bbox.bMin.y - ray.Origin.y) * divy;
		tymax = (bbox.bMax.y - ray.Origin.y) * divy;
	}
	else
	{
		tymin = (bbox.bMax.y - ray.Origin.y) * divy;
		tymax = (bbox.bMin.y - ray.Origin.y) * divy;
	}

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float divz = 1.0f / ray.Direction.z;
	if (divz >= 0.0f)
	{
		tzmin = (bbox.bMin.z - ray.Origin.z) * divz;
		tzmax = (bbox.bMax.z - ray.Origin.z) * divz;
	}
	else
	{
		tzmin = (bbox.bMax.z - ray.Origin.z) * divz;
		tzmax = (bbox.bMin.z - ray.Origin.z) * divz;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return ((tmin < t1) && (tmax > t0));
}

bool MathLib::SphereSphereIntersect( Vector3F s1, Vector3F s2, float r1, float r2 )
{
	if(distance(s1, s2) <= r1 + r2)
		return true;
	return false;
}

bool MathLib::RaySphereIntersect( Ray & r, Vector3F & s1, float r1 )
{
	Vector3F diff = s1 - r.Origin;

	if(MathLib::distance(s1, r.Origin) > r1 && MathLib::dot(diff, r.Direction) < 0.0f)
		return false;

	Vector3F res = MathLib::vec(diff, r.Direction);
	float distance = (res.length()) / (r.Direction.length());
	if (distance < r1)
		return true;
	return false;
}

// t1,t2,t3 - вершины треугольника
// l1,l2 - точки прямой (должны быть с разных сторон треугольника, иначе смысла не имеет)
// p - точка пересечения прямой и треугольника
// функция возвращает true в случае пересечения и false иначе, в p - точка пересечения
bool MathLib::LineTriangleIntersectionPoint(const Vector3F &t1, const Vector3F &t2, const Vector3F &t3,
							 const Vector3F &l1, const Vector3F &l2, Vector3F &p)
{
	Vector3F n = Normalize(vec(t2 - t1, t3 - t2));
	float d1 = dot((l1 - t1), n) / n.length();
	float d2 = dot((l2 - t1), n) / n.length();
	if ((d1 > 0 && d2 > 0) || (d1 < 0 && d2 < 0))
		return false;
	p = l1 + (l2 - l1) * (-d2 / (d2 - d1));
	if ( dot((vec(t2 - t1, p - t1)), n) <= 0 )
		return false;
	if ( dot((vec(t3 - t2, p - t2)), n) <= 0 )
		return false;
	if ( dot((vec(t1 - t3, p - t3)), n) <= 0 )
		return false;
	return true;
}