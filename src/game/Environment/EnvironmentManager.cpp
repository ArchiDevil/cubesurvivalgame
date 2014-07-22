#include "EnvironmentManager.h"

cEnvironmentManager::cEnvironmentManager()
{
	Sky = new SkyManager;
}

cEnvironmentManager::~cEnvironmentManager()
{
	if(Sky)
		delete Sky;
}

void cEnvironmentManager::Initialize(dayTimer & initialTime)
{
	this->time.setTime(initialTime.getHours(), initialTime.getMinutes());
	Sky->Initialize();
}

void cEnvironmentManager::SetTime(dayTimer & t)
{
	time = t;
}

dayTimer cEnvironmentManager::GetTime()
{
	return time;
}

void cEnvironmentManager::Update( double deltaTime, const Vector3F & playerPos )
{
	time.update(deltaTime);
	Sky->SetSunPos(calculateSunPos(playerPos));
	Sky->Update(deltaTime, playerPos);
}

SkyManager * cEnvironmentManager::GetSkyPtr()
{
	return Sky;
}

Vector3F cEnvironmentManager::calculateSunPos(const Vector3F & playerPos) const
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
