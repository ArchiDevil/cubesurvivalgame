#pragma once

#include "dayTimer.h"

#include <MathLib/math.h>

#include <GraphicsEngine/ShiftEngine.h>

// Stores current time, day, month, year.
// Animates day/night based on current time.
// Changes weather conditions and seasons.

class EnvironmentManager
{
public:
    EnvironmentManager();
    ~EnvironmentManager();

    void Initialize(const DayTimer & initialTime);
    void Update(double deltaTime);
    void SetTime(const DayTimer & t);
    DayTimer GetTime() const;
    MathLib::Vector3F GetSunPosition() const;
    float GetEnvironmentTemperature() const;

private:
    DayTimer time;

    float minimalTemperature; // temperature at the midnight
    float maximalTemperature; // temperature at the midday

    ShiftEngine::LightNode * pSun = nullptr;

};
