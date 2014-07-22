#pragma once

#include "../../MathLib/math.h"
#include "../../GraphicsEngine/SceneGraph/MeshNode.h"

class SkyManager
{
public:
	void Initialize();
	void Update(double dt, const Vector3F & camPos);

	Vector3F GetSunPos() const;
	float getSunIntensity() const;
	
	void SetSunPos(const Vector3F & pos);

private:
	Vector3F sunPos;
	ShiftEngine::LightNode * sunNode;

};