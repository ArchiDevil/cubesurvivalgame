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

		quaternion(T _x = 0.0, T _y = 0.0, T _z = 0.0, T _w = 1.0)
			: vector(_x, _y, _z), w(_w) {}

		quaternion(const vec3<T> & axis, T angle)
			: vector(axis * sin(angle / 2.0f)), w(cos(angle / 2.0f))
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
			float sin_a = sin( angle / 2 );
			float cos_a = cos( angle / 2 );

			float sin_lat = sin( latitude );
			float cos_lat = cos( latitude );

			float sin_long = sin( longitude );
			float cos_long = cos( longitude );

			vector.x = sin_a * cos_lat * sin_long;
			vector.y = sin_a * sin_lat;
			vector.z = sin_a * sin_lat * cos_long;
			w = cos_a;
		}

		quaternion(const matrix<T, 3> & ref)
		{
			float  tr, s, q[4];
			int    i, j, k;

			int nxt[3] = {1, 2, 0};

			tr = ref.arr[0][0] + ref.arr[1][1] + ref.arr[2][2];

			if (tr > 0.0)
			{
				s = sqrt (tr + 1.0);
				w = s / 2.0;
				s = 0.5 / s;
				vector.x = (ref.arr[1][2] - ref.arr[2][1]) * s;
				vector.y = (ref.arr[2][0] - ref.arr[0][2]) * s;
				vector.z = (ref.arr[0][1] - ref.arr[1][0]) * s;
			}
			else
			{
				i = 0;
				if (ref.arr[1][1] > ref.arr[0][0]) i = 1;
				if (ref.arr[2][2] > ref.arr[i][i]) i = 2;
				j = nxt[i];
				k = nxt[j];

				s = sqrt ((ref.arr[i][i] - (ref.arr[j][j] + ref.arr[k][k])) + 1.0);

				q[i] = s * 0.5;

				if (s != 0.0) s = 0.5 / s;

				q[3] = (ref.arr[j][k] - ref.arr[k][j]) * s;
				q[j] = (ref.arr[i][j] + ref.arr[j][i]) * s;
				q[k] = (ref.arr[i][k] + ref.arr[k][i]) * s;

				vector.x = q[0];
				vector.y = q[1];
				vector.z = q[2];
				w = q[3];
			}
		}

		/*
		* Смысл операции сложения можно описать как "смесь" вращений, т.е. мы получим вращение, которое находится между q и  q'.
		* Умножение на скаляр на вращении не отражается.
		* Кватернион, умноженный на скаляр, представляет то же самое вращение, кроме случая умножения на 0.
		* При умножении на 0 мы получим "неопределенное" вращение.
		*/
		quaternion operator + (const quaternion & ref) const
		{
			return quaternion(this->vector + ref.vec, this->w + ref.w);
		}

		quaternion operator - (const quaternion & ref) const
		{
			return quaternion(this->vector - ref.vec, this->w - ref.w);
		}

		quaternion operator * (const T scalar) const
		{
			return quaternion(this->vector * scalar, this->w * scalar);
		}

		quaternion operator / (const T scalar) const
		{
			return quaternion(this->vector / scalar, this->w / scalar);
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
			return quaternion(-vector, this->w);
		}

		/*
		* Существует такой кватернион, при умножении на который произведение дает нулевое вращение и соответствующее тождественному кватерниону (identity quaternion):
		*/
		quaternion inverse() const
		{
			return conjugate() / norm();
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

		inline void to_matrix( matrix<float, 3> & m ) const
		{
			float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
			float s  = 2.0f / norm();  // 4 mul 3 add 1 div
			x2 = vector.x * s;    y2 = vector.y * s;    z2 = vector.z * s;
			xx = vector.x * x2;   xy = vector.x * y2;   xz = vector.x * z2;
			yy = vector.y * y2;   yz = vector.y * z2;   zz = vector.z * z2;
			wx = w * x2;   wy = w * y2;   wz = w * z2;

			m.arr[0][0] = 1.0f - (yy + zz);
			m.arr[1][0] = xy - wz;
			m.arr[2][0] = xz + wy;

			m.arr[0][1] = xy + wz;
			m.arr[1][1] = 1.0f - (xx + zz);
			m.arr[2][1] = yz - wx;

			m.arr[0][2] = xz - wy;
			m.arr[1][2] = yz + wx;
			m.arr[2][2] = 1.0f - (xx + yy);
		}
	};

	typedef quaternion<float> qaFloat;
	typedef quaternion<double> qaDouble;
}
