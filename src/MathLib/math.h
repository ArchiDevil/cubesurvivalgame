#pragma once

#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "math/AABB.h"
#include "math/ray.h"
#include "math/intersections.h"
#include "math/plane.h"
#include "math/quaternion.h"
#include "math/matrix.h"

#include "math/matrixFuncs.h"
#include "math/vectorsFuncs.h"

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
    #define M_PIF 3.14159265f
#endif

namespace MathLib
{
    template<typename T>
    T clamp(T value, T min, T max)
    {
        if (value > max)
            return max;
        if (value < min)
            return min;
        return value;
    }

    //returns angle between vector and X-axis
    template<typename T>
    T angleX(const vec2<T> & vec)
    {
        vec2<T> tempVec = normalize(vec);
        if (asin(tempVec.y) >= 0)
            return acos(tempVec.x);
        else
            return -acos(tempVec.x);
    }

    template<typename T>
    inline T LinearInterpolation(T a, T b, T k)
    {
        return a + (b - a) * k;
    }

    template<typename T>
    inline T CosineInterpolation(T a, T b, T k)
    {
        T ft = k * (T)M_PI;
        T f = (1 - std::cos(ft)) * 0.5f;
        return a*(1 - f) + b*f;
    }

    template<typename T>
    inline vec3<T> GetPointOnSphere(vec3<T> center, T radius, T xAngle, T yAngle)
    {
        T x = radius * std::sin(yAngle * (T)0.0175) * std::cos(xAngle * (T)0.0175) + center.x;
        T y = radius * std::sin(yAngle * (T)0.0175) * std::sin(xAngle * (T)0.0175) + center.y;
        T z = radius * std::cos(yAngle * (T)0.0175) + center.z;
        return vec3<T>(x, y, z);
    }

    template<typename T>
    T degrad(T degress)
    {
        return degress * (T)M_PI / (T)180.0;
    }

    template<typename T>
    T raddeg(T radians)
    {
        return radians * (T)180.0 / (T)M_PI;
    }
}
