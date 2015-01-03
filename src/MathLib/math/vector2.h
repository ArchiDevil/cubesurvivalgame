#pragma once

#include <cmath>

namespace MathLib
{
	template<typename T>
	struct vec2
	{
		vec2() : x(0), y(0) {};
		vec2(T _x, T _y) : x(_x), y(_y) {};

		union
		{
			T el[2];
			struct
			{
				T x, y;
			};
		};

		T length() const
		{
			return (T)sqrt((double)x*x + (double)y*y);
		}

		inline const T * ptr() const
		{
			return el;
		}

		inline T * ptr()
		{
			return el;
		}

		inline vec2 & operator += (const vec2 & ref)
		{
			this->x += ref.x;
			this->y += ref.y;
			return *this;
		}

		inline vec2 & operator -= (const vec2 & ref)
		{
			this->x -= ref.x;
			this->y -= ref.y;
			return *this;
		}

		template<typename D>
		inline vec2 & operator *= (D num)
		{
			this->x *= (T)num;
			this->y *= (T)num;
			return *this;
		}

		template<typename T2>
		inline vec2 & operator = (vec2<T2> & vec)
		{
			this->x = (T)vec.x;
			this->y = (T)vec.y;
			return *this;
		}

		inline vec2 operator + (const vec2 & ref) const
		{
			return vec2(this->x + ref.x, this->y + ref.y);
		}

		inline vec2 operator - (const vec2 & ref) const
		{
			return vec2(this->x - ref.x, this->y - ref.y);
		}

		inline vec2 operator - () const
		{
			return vec2(-x, -y);
		}

		template<typename T2> operator vec2<T2>() const
		{
			return vec2<T2>((T2)x, (T2)y);
		}

		template<typename D>
		inline vec2 operator * (D num) const
		{
			return vec2(x * (T)num, y * (T)num);
		}

		inline bool operator == (const vec2 & ref) const
		{
			return ((this->x == ref.x) && (this->y == ref.y));
		}

		inline bool operator != (const vec2 & ref) const
		{
			return !((this->x == ref.x) && (this->y == ref.y));
		}
	};

	typedef vec2<double>	Vector2D;
	typedef vec2<float>		Vector2F;
	typedef vec2<int>		Vector2I;
}