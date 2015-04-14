#pragma once

#include "vectorsFuncs.h"

namespace MathLib
{
    template<typename T>
    struct plane
    {
        plane(T a = (T)0.0, T b = (T)0.0, T c = (T)0.0, T d = (T)0.0)
            : a(a)
            , b(b)
            , c(c)
            , d(d)
        {
        }

        bool operator == (const plane & ref) const
        {
            return (a == ref.a) && (b == ref.b) && (c == ref.c) && (d == ref.d);
        }

        bool operator != (const plane & ref) const
        {
            return (*this == ref);
        }

        T a, b, c, d;
    };

    template<typename T>
    plane<T> normalize(const plane<T> & p)
    {
        vec3<T> n = normalize<T>(vec3<T>(p.a, p.b, p.c));
        return{ n.x, n.y, n.z, p.d };
    }

    template<typename T>
    T planeDotCoord(const plane<T> & p, const vec3<T> & v)
    {
        return p.a * v.x + p.b * v.y + p.c * v.z + p.d * (T)1.0;
    }

    template<typename T>
    T planeDotNormal(const plane<T> & p, const vec3<T> & v)
    {
        return p.a * v.x + p.b * v.y + p.c * v.z + p.d * 0.0;
    }
}
