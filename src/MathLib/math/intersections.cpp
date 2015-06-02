#include "intersections.h"

#include "../math.h"

// ray - ray which check to intersection
// bbox - bounding box to check
// t0, t1 - start/end points on ray
// function returns true on intersection and false if not

bool MathLib::RayBoxIntersect(const Ray & ray, const AABB & bbox, float t0, float t1)
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


// s1, s2 - centers of spheres to test
// r1, r2 - radiuses of spheres
// function returns true on intersection and false if not

bool MathLib::SphereSphereIntersect(Vector3F s1, Vector3F s2, float r1, float r2)
{
    return distance(s1, s2) <= r1 + r2;
}


// ray - ray which check to intersection
// s1 - sphere center
// r1 - sphere radius
// function returns true on intersection and false if not

bool MathLib::RaySphereIntersect(const Ray & r, const Vector3F & s1, float r1)
{
    Vector3F diff = s1 - r.Origin;

    if (MathLib::distance(s1, r.Origin) > r1 && MathLib::dot(diff, r.Direction) < 0.0f)
        return false;

    Vector3F res = MathLib::cross(diff, r.Direction);
    float distance = (res.length()) / (r.Direction.length());
    if (distance < r1)
        return true;

    return false;
}

// t1, t2, t3 - triangle vertices
// l1,l2 - line start/end points
// p - intersection point
// function returns true on intersection and false if not

bool MathLib::LineTriangleIntersectionPoint(const Vector3F &t1, const Vector3F &t2, const Vector3F &t3,
                                            const Vector3F &l1, const Vector3F &l2, Vector3F &p)
{
    // normal calculation
    Vector3F n = normalize(cross(t2 - t1, t3 - t2));

    // distances to check
    float d1 = dot((l1 - t1), n) / n.length();
    float d2 = dot((l2 - t1), n) / n.length();

    // both points on the one side or line is parallel to triangle
    if (d1 * d2 > 0.0f || d1 == d2)
        return false;

    // intersection point
    p = l1 + (l2 - l1) * (-d1 / (d2 - d1));

    // test result point to check in lies inside triangle
    if (dot(cross(t2 - t1, p - t1), n) <= 0)
        return false;

    if (dot(cross(t3 - t2, p - t2), n) <= 0)
        return false;

    if (dot(cross(t1 - t3, p - t3), n) <= 0)
        return false;

    return true;
}
