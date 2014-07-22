#pragma once

namespace MathLib
{
	//graphics matrix
	template<typename T, unsigned int E>
	struct matrix
	{
		T arr[E][E];

		matrix()
		{
			for (int i = 0; i < E ; i++)
				for (int j = 0; j < E ; j++)
					arr[i][j] = 0;
		}

		matrix(const matrix & ref)
		{
			for (int i = 0; i < E ; i++)
				for (int j = 0; j < E ; j++)
					arr[i][j] = ref.arr[i][j];
		}

		matrix operator + ( const matrix & ref ) const
		{
			matrix out;
			for (int i = 0; i < E ; i++)
				for (int j = 0; j < E ; j++)
					out.arr[i][j] = this->arr[i][j] + ref.arr[i][j];

			return out;
		}

		matrix operator - ( const matrix & ref ) const
		{
			matrix out;
			for (int i = 0; i < E ; i++)
				for (int j = 0; j < E ; j++)
					out.arr[i][j] = this->arr[i][j] - ref.arr[i][j];

			return out;
		}

		vec3<T> operator * ( const vec3<T> & ref ) const
		{
			//static_assert(E == 3, "Unable to multiply matrices with different sizes");
			const int r = 3;	//number of rows in 2nd and columns in 1st matrices
			const int m = E;	//number of rows in 1st matrix
			vec3<T> out;

			for (int i = 0; i < m; i++)
			{
				float summ = 0;
				for (int z = 0; z < r ; z++)
				{
					summ += this->arr[i][z] * ref.el[z];
				}
				out.el[i] = summ;
			}
			return out;
		}

		matrix<T, E> operator * ( const matrix<T, E> & ref ) const
		{
			const int r = E;	//number of rows in 2nd and columns in 1st matrices
			const int m = E;	//number of rows in 1st matrix
			const int q = E;	//number of columns in 2nd matrix
			matrix temp;

			for (int i = 0; i < m; i++)
			{
				for (int j = 0; j < q; j++)
				{
					float summ = 0;
					for (int z = 0; z < r ; z++)
					{
						summ += this->arr[i][z] * ref.arr[z][j];
					}
					temp.arr[i][j] = summ;
				}
			}
			return temp;
		}

		matrix operator + ( const T scalar ) const
		{
			matrix out;
			for (int i = 0; i < E ; i++)
				for (int j = 0; j < E ; j++)
					out.arr[i][j] = this->arr[i][j] + scalar;

			return out;
		}

		matrix operator - ( const T scalar ) const
		{
			matrix out;
			for (int i = 0; i < E ; i++)
				for (int j = 0; j < E ; j++)
					out.arr[i][j] = this->arr[i][j] - scalar;

			return out;
		}

		matrix operator * ( const T scalar ) const
		{
			matrix out;
			for (int i = 0; i < E ; i++)
				for (int j = 0; j < E ; j++)
					out.arr[i][j] = this->arr[i][j] * scalar;

			return out;
		}

		matrix operator / ( const T scalar ) const
		{
			matrix out;
			for (int i = 0; i < E ; i++)
				for (int j = 0; j < E ; j++)
					out.arr[i][j] = this->arr[i][j] / scalar;

			return out;
		}

		matrix & operator = ( const matrix & ref )
		{
			for (int i = 0; i < E ; i++)
				for (int j = 0; j < E ; j++)
					arr[i][j] = ref.arr[i][j];

			return *this;
		}

		T * operator [] ( const int scalar )
		{
			//returns pointer to second array
			return arr[scalar];
		}

		const T * operator [] (const int scalar) const
		{
			return arr[scalar];
		}

		bool operator == (const matrix & ref) const
		{
			for(int i = 0; i < E; i++)
				for(int j = 0; j < E; j++)
					if(this->arr[i][j] != ref.arr[i][j])
						return false;

			return true;
		}

		bool operator != (const matrix & ref) const
		{
			return !(*this == ref);
		}

		matrix<T, E> transpose() const
		{
			matrix<T, E> out;
			for (int i = 0; i < E; i++)
			{
				for (int j = 0; j < E ; j++)
				{
					out[i][j] = arr[j][i];
				}
			}
			return out;
		}
	};

	typedef matrix<float, 3> mat3f;
	typedef matrix<double, 3> mat3d;
	typedef matrix<float, 4> mat4f;
	typedef matrix<double, 4> mat4d;
}
