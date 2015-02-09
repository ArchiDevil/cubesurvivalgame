#pragma once

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

namespace MathLib
{
	template <typename T>
	T dot(const vec2<T> & vec1, const vec2<T> & vec2)
	{
		return vec1.x*vec2.x + vec1.y*vec2.y;
	}

	template <typename T>
	T dot(const vec3<T> & vec1, const vec3<T> & vec2)
	{
		return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z;
	}

	template <typename T>
	T dot(const vec4<T> & vec1, const vec4<T> & vec2)
	{
		return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z + vec1.w*vec2.w;
	}

	template<typename T>
	vec3<T> vec(const vec3<T> & vec1, const vec3<T> & vec2)
	{
		return vec3<T>(vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x);
	}

	template<typename T>
	vec2<T> Normalize(const vec2<T> & vec)		//нормализует вектор
	{
		T inv_length = (T)1.0 / vec.length();
		return vec * inv_length;
	}

	template<typename T>
	vec3<T> Normalize(const vec3<T> & vec)
	{
		T inv_length = (T)1.0 / vec.length();
		return vec * inv_length;
	}

	template<typename T>
	vec4<T> Normalize(const vec4<T> & vec)
	{
		T inv_length = (T)1.0 / vec.length();
		return vec * inv_length;
	}

	//угол между векторами
	template<typename T>
	T angle(const vec2<T> &vec1, const vec2<T> &vec2)
	{
		return acos((T)dot(vec1, vec2) / (T)vec1.length() / (T)vec2.length());
	}

	//угол между векторами
	template<typename T>
	T angle(const vec3<T> &vec1, const vec3<T> &vec2)
	{
		return acos((T)dot(vec1, vec2) / (T)vec1.length() / (T)vec2.length());
	}

	//угол между векторами
	template<typename T>
	T angle(const vec4<T> &vec1, const vec4<T> &vec2)
	{
		return acos((T)dot(vec1, vec2) / (T)vec1.length() / (T)vec2.length());
	}

	//возвращает единичный вектор, спроецированный на X
	template<typename T>
	vec3<T> projX(const vec3<T> &vec)
	{
		vec3<T> out = vec;
		out.x = 0.0f;
		out = Normalize(out);
		return out;
	}

	//возвращает единичный вектор, спроецированный на Y
	template<typename T>
	vec3<T> projY(const vec3<T> &vec)
	{
		vec3<T> out = vec;
		out.y = 0.0f;
		out = Normalize(out);
		return out;
	}

	//возвращает единичный вектор, спроецированный на Z
	template<typename T>
	vec3<T> projZ(const vec3<T> &vec)
	{
		vec3<T> out = vec;
		out.z = 0.0f;
		out = Normalize(out);
		return out;
	}

	template<typename T>
	T distance(const vec2<T> & arg1, const vec2<T> & arg2)
	{
		return (T)sqrt((T)(arg1.x - arg2.x)*(arg1.x - arg2.x) +
			(T)(arg1.y - arg2.y)*(arg1.y - arg2.y));
	}

	template<typename T>
	T distance(const vec3<T> & vec1, const vec3<T> & vec2)
	{
		return (T)sqrt((T)(vec1.x - vec2.x)*(vec1.x - vec2.x) +
			(T)(vec1.y - vec2.y)*(vec1.y - vec2.y) +
			(T)(vec1.z - vec2.z)*(vec1.z - vec2.z));
	}

	template<typename T>
	T distance(const vec4<T> & vec1, const vec4<T> & vec2)
	{
		return (T)sqrt((T)(vec1.x - vec2.x)*(vec1.x - vec2.x) +
			(T)(vec1.y - vec2.y)*(vec1.y - vec2.y) +
			(T)(vec1.z - vec2.z)*(vec1.z - vec2.z) +
			(T)(vec1.w - vec2.w)*(vec1.w - vec2.w));
	}

	template<typename T>
	vec3<T> getUnprojectedVector(const vec3<T> & v, const matrix<T, 4> & projMat, const matrix<T, 4> & viewMat, const vec2<unsigned int> & screenSize)
	{
		matrix<T, 4> rm = viewMat * projMat;
		matrix<T, 4> resultMatrix = matrixInverse(rm);
		Vector3F result;

		int TopLeftX = 0;
		int TopLeftY = 0;
        unsigned int Width = screenSize.x;
        unsigned int Height = screenSize.y;
		float MinDepth = 0.0f;
		float MaxDepth = 1.0f;

		result.x = (v.x - TopLeftX) / Width * 2.0f - 1.0f;
		result.y = -(2.0f * (v.y - TopLeftY) / Height - 1.0f);
		result.z = (v.z - MinDepth) / (MaxDepth - MinDepth);
		result = vec3Transform(result, resultMatrix);
		return result;
	}
}
