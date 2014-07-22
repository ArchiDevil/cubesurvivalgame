#pragma once

#include <cmath>

namespace MathLib
{

	template<typename T>
	struct vec4
	{
		vec4() : x(0), y(0), z(0), w(0){};
		vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w){};

		union
		{
			T el[4];
			struct
			{
				T x, y, z, w;
			};
		};

		T length() const
		{
			return (T)sqrt((double)x*x + (double)y*y + (double)z*z + (double)w*w);
		}

		inline T * ptr()
		{
			return el;
		}

		inline vec4 & operator += (const vec4 & ref)
		{
			this->x += ref.x;
			this->y += ref.y;
			this->z += ref.z;
			this->w += ref.w;
			return *this;
		}

		inline vec4 & operator -= (const vec4 & ref)
		{
			this->x -= ref.x;
			this->y -= ref.y;
			this->z -= ref.z;
			this->w -= ref.w;
			return *this;
		}

		inline vec4 & operator *= (float num)
		{
			this->x *= num;
			this->y *= num;
			this->z *= num;
			this->w *= num;
			return *this;
		}

		inline vec4 & operator *= (double num)
		{
			this->x *= num;
			this->y *= num;
			this->z *= num;
			this->w *= num;
			return *this;
		}

		inline vec4 & operator = (const vec4 & vec)
		{
			this->x = vec.x;
			this->y = vec.y;
			this->z = vec.z;
			this->w = vec.w;
			return *this;
		}

		inline vec4 operator + (const vec4 & ref)
		{
			return vec4(this->x + ref.x, this->y + ref.y, this->z + ref.z, this->w + ref.w);
		}

		inline vec4 operator - (const vec4 & ref)
		{
			return vec4(this->x - ref.x, this->y - ref.y, this->z - ref.z, this->w - ref.w);
		}

		inline vec4 operator * (float num)
		{
			return vec4(this->x * num, this->y * num, this->z * num, this->w * num);
		}

		inline vec4 operator * (double num)
		{
			return vec4(this->x * num, this->y * num, this->z * num, this->w * num);
		}

		inline bool operator == (const vec4 & ref) const
		{
			return ((this->x == ref.x) && (this->y == ref.y) && (this->z == ref.z) && (this->w == ref.w));
		}

		inline bool operator != (const vec4 & ref) const
		{
			return !((this->x == ref.x) && (this->y == ref.y) && (this->z == ref.z) && (this->w == ref.w));
		}
	};

	typedef vec4<float>  Vector4F;
	typedef vec4<double> Vector4D;
}