#pragma once

#include "Sky.h"
#include "../dayTimer.h"

// Хранит текущее время, день, месяц, год.
// На основе этого анимирует небо, подсчитывает время,
// меняет погодные условия и времена года для окружения.

class cEnvironmentManager
{
public:
	cEnvironmentManager();
	~cEnvironmentManager();

	void Initialize(dayTimer & initialTime);
	void Update(double deltaTime, const Vector3F & playerPos);
	
	void SetTime(dayTimer & t);
	dayTimer GetTime();
	
	SkyManager * GetSkyPtr();

private:
	Vector3F calculateSunPos(const Vector3F & playerPos) const;

	SkyManager * Sky;
	dayTimer time;

};