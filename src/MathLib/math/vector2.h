#pragma once

#include <cmath>

namespace MathLib
{
	template<typename T>
	struct vec2
	{
		vec2() : x(0), y(0){};
		vec2(T _x, T _y) : x(_x), y(_y){};

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

		inline vec2 & operator *= (float num)
		{
			this->x *= num;
			this->y *= num;
			return *this;	
		}

		inline vec2 & operator *= (double num)
		{
			this->x *= num;
			this->y *= num;
			return *this;	
		}

		inline vec2 & operator = (const vec2 & vec)
		{
			this->x = vec.x;
			this->y = vec.y;
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

		inline vec2 operator * (float num) const
		{
			return vec2(this->x * num, this->y * num);
		}

		inline vec2 operator * (double num) const
		{
			return vec2(this->x * num, this->y * num);
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