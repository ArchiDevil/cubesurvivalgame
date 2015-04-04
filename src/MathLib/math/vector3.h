#pragma once

#include <cmath>

namespace MathLib
{
    template<typename T>
    struct vec3
    {
        vec3()
            : x(0)
            , y(0)
            , z(0)
        {
        }

        vec3(T _x, T _y, T _z)
            : x(_x)
            , y(_y)
            , z(_z)
        {
        }

        union
        {
            T el[3];
            struct
            {
                T x, y, z;
            };
        };

        T length() const
        {
            return (T)sqrt((double)x*x + (double)y*y + (double)z*z);
        }

        inline T * ptr()
        {
            return el;
        }

        inline const T * ptr() const
        {
            return el;
        }

        inline vec3 & operator += (const vec3 & ref)
        {
            this->x += ref.x;
            this->y += ref.y;
            this->z += ref.z;
            return *this;
        }

        inline vec3 & operator -= (const vec3 & ref)
        {
            this->x -= ref.x;
            this->y -= ref.y;
            this->z -= ref.z;
            return *this;
        }

        template<typename D>
        inline vec3 & operator *= (D num)
        {
            this->x *= (T)num;
            this->y *= (T)num;
            this->z *= (T)num;
            return *this;
        }

        template<typename T2>
        inline vec3 & operator = (const vec3<T2> & vec)
        {
            this->x = (T)vec.x;
            this->y = (T)vec.y;
            this->z = (T)vec.z;
            return *this;
        }

        template<typename T2> operator vec3<T2>() const
        {
            return vec3<T2>((T2)x, (T2)y, (T2)z);
        }

        inline vec3 operator + (const vec3 & ref) const
        {
            return vec3(this->x + ref.x, this->y + ref.y, this->z + ref.z);
        }

        inline vec3 operator - (const vec3 & ref) const
        {
            return vec3(this->x - ref.x, this->y - ref.y, this->z - ref.z);
        }

        inline vec3 operator - () const
        {
            return vec3(-x, -y, -z);
        }

        template<typename D>
        inline vec3 operator * (D num) const
        {
            return vec3(x * (T)num, y * (T)num, z * (T)num);
        }

        inline bool operator == (const vec3 & ref) const
        {
            return ((this->x == ref.x) && (this->y == ref.y) && (this->z == ref.z));
        }

        inline bool operator != (const vec3 & ref) const
        {
            return !((this->x == ref.x) && (this->y == ref.y) && (this->z == ref.z));
        }
    };

    typedef vec3<float>     Vector3F;
    typedef vec3<double>    Vector3D;
    typedef vec3<int>       Vector3I;
}
