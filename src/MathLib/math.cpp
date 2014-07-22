#include "math.h"

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

float MathLib::LinearInterpolation( float a, float b, float k )
{
	return a + (b - a) * k;
}

float MathLib::round( float num )
{
	return (num - floor(num) >= 0.5f) ? ceil(num) : floor(num);
}

float MathLib::CosineInterpolation( float a, float b, float k )
{
	float ft = k * 3.1415927f;
	float f = (1 - cos(ft)) * 0.5f;
	return  a*(1-f) + b*f;
}

MathLib::Vector3F MathLib::GetPointOnSphere( Vector3F center, float R, float xAngle, float yAngle )
{
	float x = R * sin(yAngle * 0.0175f) * cos(xAngle * 0.0175f) + center.x;
	float y = R * sin(yAngle * 0.0175f) * sin(xAngle * 0.0175f) + center.y;
	float z = R * cos(yAngle * 0.0175f) + center.z;
	return Vector3F(x, y, z);
}

double MathLib::degrad( double degress )
{
	return degress * M_PI / 180.0;
}

double MathLib::raddeg( double radians )
{
	return radians * 180.0 / M_PI;
}
