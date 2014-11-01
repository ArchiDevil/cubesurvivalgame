#pragma once

#include "../dayTimer.h"
#include <MathLib/math.h>

// Хранит текущее время, день, месяц, год.
// На основе этого анимирует небо, подсчитывает время,
// меняет погодные условия и времена года для окружения.

class cEnvironmentManager
{
public:
	cEnvironmentManager();
	~cEnvironmentManager();

	void Initialize(dayTimer & initialTime);
	void Update(double deltaTime);
	
	void SetTime(dayTimer & t);
	dayTimer GetTime() const;
	MathLib::Vector3F calculateSunPos(const MathLib::Vector3F & playerPos) const;

private:
	dayTimer time;

};