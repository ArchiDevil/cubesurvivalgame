#pragma once

namespace MathLib
{
	struct Plane
	{
		Plane(float _A = 0.0f, float _B = 0.0f, float _C = 0.0f, float _D = 0.0f) 
			: A(_A), B(_B), C(_C), D(_D) {}

		bool operator == ( const Plane & ref ) const
		{
			return (A == ref.A) && (B == ref.B) && (C == ref.C) && (D == ref.D);
		}

		bool operator != ( const Plane & ref ) const
		{
			return (*this == ref);
		}

		float A, B, C, D;
	};
}