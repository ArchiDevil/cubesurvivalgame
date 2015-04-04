#include "math.h"

float MathLib::LinearInterpolation(float a, float b, float k)
{
    return a + (b - a) * k;
}

float MathLib::CosineInterpolation(float a, float b, float k)
{
    float ft = k * 3.1415927f;
    float f = (1 - cos(ft)) * 0.5f;
    return  a*(1 - f) + b*f;
}

MathLib::Vector3F MathLib::GetPointOnSphere(Vector3F center, float R, float xAngle, float yAngle)
{
    float x = R * sin(yAngle * 0.0175f) * cos(xAngle * 0.0175f) + center.x;
    float y = R * sin(yAngle * 0.0175f) * sin(xAngle * 0.0175f) + center.y;
    float z = R * cos(yAngle * 0.0175f) + center.z;
    return Vector3F(x, y, z);
}
