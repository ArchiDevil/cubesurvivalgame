#pragma once

#include "dayTimer.h"

#include <MathLib/math.h>

// Stores current time, day, month, year.
// Animates day/night based on current time.
// Changes weather conditions and seasons.

class EnvironmentManager
{
public:
    EnvironmentManager();
    ~EnvironmentManager();

    void Initialize(const dayTimer & initialTime);
    void Update(double deltaTime);
    void SetTime(const dayTimer & t);
    dayTimer GetTime() const;
    MathLib::Vector3F GetSunPosition(const MathLib::Vector3F & playerPos) const;
    float GetEnvironmentTemperature() const;

private:
    dayTimer time;

    float minimalTemperature; // temperature at the midnight
    float maximalTemperature; // temperature at the midday

};
