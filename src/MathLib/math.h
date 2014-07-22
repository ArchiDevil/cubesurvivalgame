#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

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

namespace MathLib
{
	template<typename T>
	T clamp(T value, T min, T max)
	{
		if(value > max)
			return max;
		if(value < min)
			return min;
		return value;
	}

	//returns angle between vector and X-axis
	template<typename T>
	T angleX(const vec2<T> & vec)
	{
		vec2<T> tempVec = Normalize(vec);
		if(asin(tempVec.y) >= 0)
			return acos(tempVec.x);
		else
			return -acos(tempVec.x);
	}

	float LinearInterpolation(float a, float b, float k);			//линейна€ интерпол€ци€
	float CosineInterpolation(float a, float b, float k);			//косинусна€ интерпол€ци€

	float round(float num);											//округление по математическим правилам

	Vector3F GetPointOnSphere(Vector3F center, float radius, float xAngle, float yAngle);	//точка на сфере

	double degrad(double degress);
	double raddeg(double radians);
}
