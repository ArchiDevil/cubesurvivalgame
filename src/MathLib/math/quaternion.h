#pragma once

#include "../math.h"
#include "vector3.h"

/*
 * Преобразование улов Эйлера в кватернион:
 * D3DXQuaternionRotationYawPitchRoll()
 *
 * Сферическая линейная интерполяция:
 * D3DXQuaternionSlerp()
 */

namespace MathLib
{
	template<typename T>
	class quaternion
	{
	public:
		vec3<T> vector;
		T w;

		quaternion()
			: vector(T(1.0), T(0.0), T(0.0))
			, w(T(1.0))
		{
		}

		quaternion(T x, T y, T z, T w)
			: vector(x, y, z)
			, w(w) 
		{
		}

		quaternion(const vec3<T> & vec)
			: vector(vec)
			, w(0)
		{
		}

		/*
		qroll =  [cos (y/2), (sin(y/2), 0, 0)]
		qpitch = [cos (q/2), (0, sin(q/2), 0)]
		qyaw =  [cos (f/2), (0, 0, sin(f/2))]
		q = qyaw qpitch qroll
		*/

		quaternion(float latitude, float longitude, float angle)
		{
			float sin_a = sin(angle / 2);
			float cos_a = cos(angle / 2);

			float sin_lat = sin(latitude);
			float cos_lat = cos(latitude);

			float sin_long = sin(longitude);
			float cos_long = cos(longitude);

			vector.x = sin_a * cos_lat * sin_long;
			vector.y = sin_a * sin_lat;
			vector.z = sin_a * sin_lat * cos_long;
			w = cos_a;
		}

		/*
		* Смысл операции сложения можно описать как "смесь" вращений, т.е. мы получим вращение, которое находится между q и  q'.
		* Умножение на скаляр на вращении не отражается.
		* Кватернион, умноженный на скаляр, представляет то же самое вращение, кроме случая умножения на 0.
		* При умножении на 0 мы получим "неопределенное" вращение.
		*/
		quaternion operator + (const quaternion & ref) const
		{
			vec3<T> result_vec = this->vector + ref.vector;
			return quaternion(result_vec.x, result_vec.y, result_vec.z, this->w + ref.w);
		}

		quaternion operator - (const quaternion & ref) const
		{
			vec3<T> result_vec = this->vector - ref.vector;
			return quaternion(result_vec.x, result_vec.y, result_vec.z, this->w - ref.w);
		}

		/*
		* Одна из самых полезных операций, она аналогична умножению двух матриц поворота.
		* Итоговый кватернион представляет собой комбинацию вращений — объект повернули на q' а затем на q (если смотреть из глобальной системы координат).
		* Кватернионы не коммутативны по умножению, как и матрица вращения: qq' не равно q'q (это свойство вращений, а не их представления).
		*/
		quaternion operator * (const quaternion & ref) const
		{
			quaternion<T> out;
			out.vector = vec(this->vector, ref.vector) + ref.vector * this->w + this->vector * ref.w;
			out.w = this->w * ref.w - dot(this->vector, ref.vector);
			return out;
		}

		quaternion operator * (T scalar) const
		{
			quaternion<T> out = *this;
			out.vector *= scalar;
			out.w *= scalar;
			return out;
		}

		friend vec3<T> operator * (const vec3<T> & left, const quaternion<T> & right)
		{
			quaternion<T> result = right * quaternion<T>(left) * right.inverse();
			return result.to_vector();
		}

		bool operator == (const quaternion & ref) const
		{
			return this->vector == ref.vector
				&& this->w == ref.w;
		}

		bool operator != (const quaternion & ref) const
		{
			return !(*this == ref);
		}

		T norm() const
		{
			return vector.x*vector.x + vector.y*vector.y + vector.z*vector.z + w*w;
		}

		T magnitude() const
		{
			return sqrt(norm());
		}

		/*
		* Задает вращение обратное данному.
		* Обратное вращение можно также получить, просто поменяв знак скалярного "w" компонента на обратный.
		*/
		quaternion conjugate() const
		{
			return quaternion(-vector.x, -vector.y, -vector.z, this->w);
		}

		/*
		* Существует такой кватернион, при умножении на который произведение дает нулевое вращение и соответствующее тождественному кватерниону (identity quaternion):
		*/
		quaternion inverse() const
		{
			return conjugate() * (T(1.0) / norm());
		}

		/*
		* Скалярное произведение полезно тем, что дает косинус половины угла между двумя кватернионами умноженную на их длину.
		* Соответственно скалярное произведение двух единичных кватернионов даст косинус половины угла между двумя ориентациями.
		* Угол между кватернионами это угол поворота из q  в  q' (по кратчайшей дуге).
		*/
		quaternion innerProduct(const quaternion & ref) const
		{
			return quaternion(vec3<T>(vector.x * ref.vector.x,
				vector.y * ref.vector.y,
				vector.z * ref.vector.z), w * ref.w);
		}

		quaternion normalize() const
		{
			T module = magnitude();
			return quaternion<T>(vector.x / module, vector.y / module, vector.z / module, w / module);
		}

		matrix<T, 3> to_matrix() const
		{
			float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
			float s = 2.0f / norm();  // 4 mul 3 add 1 div
			x2 = vector.x * s;    y2 = vector.y * s;    z2 = vector.z * s;
			xx = vector.x * x2;   xy = vector.x * y2;   xz = vector.x * z2;
			yy = vector.y * y2;   yz = vector.y * z2;   zz = vector.z * z2;
			wx = w * x2;   wy = w * y2;   wz = w * z2;

			matrix<T, 3> m;

			m.arr[0][0] = 1.0f - (yy + zz);
			m.arr[1][0] = xy - wz;
			m.arr[2][0] = xz + wy;

			m.arr[0][1] = xy + wz;
			m.arr[1][1] = 1.0f - (xx + zz);
			m.arr[2][1] = yz - wx;

			m.arr[0][2] = xz - wy;
			m.arr[1][2] = yz + wx;
			m.arr[2][2] = 1.0f - (xx + yy);

			return m;
		}

		vec3<T> to_vector() const
		{
			return vector;
		}
	};

	typedef quaternion<float> qaFloat;
	typedef quaternion<double> qaDouble;

	template<typename T>
	quaternion<T> quaternionFromVecAngle(const vec3<T> & axis, float angle)
	{
		quaternion<T> out;
		double sin_a = sin(angle * 0.5);
		vec3<T> vec_a = axis * (T)sin_a;
		return quaternion<T>(vec_a.x, vec_a.y, vec_a.z, cos(angle * 0.5f));
	}

	template<typename T>
	quaternion<T> quaternionFromMatrix(const matrix<T, 3> & ref)
	{
		float  tr, s, q[4];
		quaternion<T> out;

		tr = ref.arr[0][0] + ref.arr[1][1] + ref.arr[2][2];

		if (tr > 0.0)
		{
			s = sqrt(tr + 1.0);
			out.w = s / 2.0;
			s = 0.5 / s;
			out.vector.x = (ref.arr[1][2] - ref.arr[2][1]) * s;
			out.vector.y = (ref.arr[2][0] - ref.arr[0][2]) * s;
			out.vector.z = (ref.arr[0][1] - ref.arr[1][0]) * s;
		}
		else
		{
			int i = 0, j, k;
			int nxt[3] = { 1, 2, 0 };

			i = 0;
			if (ref.arr[1][1] > ref.arr[0][0]) i = 1;
			if (ref.arr[2][2] > ref.arr[i][i]) i = 2;
			j = nxt[i];
			k = nxt[j];

			s = sqrt((ref.arr[i][i] - (ref.arr[j][j] + ref.arr[k][k])) + 1.0);

			q[i] = s * 0.5;

			if (s != 0.0) s = 0.5 / s;

			q[3] = (ref.arr[j][k] - ref.arr[k][j]) * s;
			q[j] = (ref.arr[i][j] + ref.arr[j][i]) * s;
			q[k] = (ref.arr[i][k] + ref.arr[k][i]) * s;

			out.vector.x = q[0];
			out.vector.y = q[1];
			out.vector.z = q[2];
			out.w = q[3];
		}
		return out;
	}

	template<typename T>
	quaternion<T> shortest_arc(const vec3<T> & from, const vec3<T> & to)
	{
		quaternion<T> out;
		auto c = MathLib::vec(from, to);
		out.vector = c;
		out.w = MathLib::dot(from, to);
		out = out.normalize();
        out.w += 1.0f;      // reducing angle to halfangle
        if( out.w <= 0.00001f ) // angle close to PI
        {
            if((from.z * from.z) > (from.x * from.x))
			{
				out.vector.x = 0.0;
				out.vector.y = from.z;
				out.vector.z = -from.y;
			}
            else 
			{
				out.vector.x = from.y;
				out.vector.y = -from.x;
				out.vector.z = 0.0;
			}
        }
		out = out.normalize();
		return out;
	}

	template<typename T>
	quaternion<T> quaternionSlerp(const quaternion<T> & from, const quaternion<T> & to, float t)
	{
		float p1[4];
		double scale0, scale1;
		quaternion<T> out;

		// косинус угла
		double cosom = from.vector.x * to.vector.x 
			+ from.vector.y * to.vector.y 
			+ from.vector.z * to.vector.z 
			+ from.w * to.w;

		if (cosom < 0.0)
		{ 
			cosom = -cosom;
			p1[0] = -to.vector.x;
			p1[1] = -to.vector.y;
			p1[2] = -to.vector.z;
			p1[3] = -to.w;
		}
		else
		{
			p1[0] = to.vector.x;
			p1[1] = to.vector.y;
			p1[2] = to.vector.z;
			p1[3] = to.w;
		}

		if ((1.0 - cosom) > 0.001)
		{
			// стандартный случай (slerp)
			double omega = acos(cosom);
			double sinom = sin(omega);
			scale0 = sin((1.0 - t) * omega) / sinom;
			scale1 = sin(t * omega) / sinom;
		}
		else
		{        
			// если маленький угол - линейная интерполяция
			scale0 = 1.0 - t;
			scale1 = t;
		}

		out.vector.x = scale0 * from.vector.x + scale1 * p1[0];
		out.vector.y = scale0 * from.vector.y + scale1 * p1[1];
		out.vector.z = scale0 * from.vector.z + scale1 * p1[2];
		out.w = scale0 * from.w + scale1 * p1[3];

		return out;
	}
}
