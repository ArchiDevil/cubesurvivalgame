#include "EnvironmentManager.h"

#include <string>
#include <cmath>

EnvironmentManager::EnvironmentManager()
	: minimalTemperature(18.0f)
	, maximalTemperature(26.0f)
{
	// this is default temperature parameters for first milestone
}

EnvironmentManager::~EnvironmentManager()
{
}

void EnvironmentManager::Initialize(const dayTimer & initialTime)
{
	SetTime(initialTime);
}

void EnvironmentManager::SetTime(const dayTimer & t)
{
	time = t;
}

dayTimer EnvironmentManager::GetTime() const
{
	return time;
}

void EnvironmentManager::Update(double deltaTime)
{
	time.update(deltaTime);
}

MathLib::Vector3F EnvironmentManager::GetSunPosition(const MathLib::Vector3F & playerPos) const
{
	// midday at 12:00
	// dusk - 18:00
	// dawn - 6:00
	// midnight - 24:00
	
	// 0; 0; 1 at midday = yAngle == 0
	// 1; 0; 0 at dawn = yAngle == 90
	// -1; 0; 0 at dusk = yAngle == 270
	// 0; 0; -1 at midnight = yAngle == 180
	// need to rotate yAngle
	
	float yAngle = 0.0f;
	const float maxTime = 24 * 60.0f;
	float currentTime = (float)time.getRawTime() - (maxTime / 2.0f);
	yAngle = - currentTime / maxTime * 360.0f;
	
	return MathLib::GetPointOnSphere(playerPos, 10000.0f, 0.0f, yAngle);
}

float EnvironmentManager::GetEnvironmentTemperature() const
{
	float currentTemp = minimalTemperature + (maximalTemperature - minimalTemperature) * sinf((float)time.getRawTime() / (24.0f * 60.0f) * M_PIF);
	return currentTemp;
}
