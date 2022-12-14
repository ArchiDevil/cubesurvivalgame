#include "EnvironmentManager.h"

#include <GraphicsEngine/ShiftEngine.h>

#include <string>
#include <cmath>

EnvironmentManager::EnvironmentManager()
    : minimalTemperature(18.0f)
    , maximalTemperature(26.0f)
{
    // there are default temperature parameters for first milestone
}

EnvironmentManager::~EnvironmentManager()
{
    if (pSun)
        ShiftEngine::GetSceneGraph()->RemoveDirectionalLightNode(pSun);
}

void EnvironmentManager::Initialize(const DayTimer & initialTime)
{
    ShiftEngine::SceneGraph * pScene = ShiftEngine::GetSceneGraph();
    SetTime(initialTime);
    pSun = pScene->AddDirectionalLightNode({}, {0.9f, 0.9f, 0.8f});
}

void EnvironmentManager::SetTime(const DayTimer & t)
{
    time = t;
}

DayTimer EnvironmentManager::GetTime() const
{
    return time;
}

void EnvironmentManager::Update(double deltaTime)
{
    time.Update(deltaTime);
    if (pSun)
        pSun->SetDirection(-GetSunPosition());
}

MathLib::Vector3F EnvironmentManager::GetSunPosition() const
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

    const float maxTime = 24 * 60.0f;
    float currentTime = (float)time.GetRawTime() - (maxTime / 2.0f);
    float yAngle = -currentTime / maxTime * 360.0f;
    return MathLib::GetPointOnSphere({}, 10000.0f, 0.0f, yAngle);
}

float EnvironmentManager::GetEnvironmentTemperature() const
{
    return minimalTemperature + (maximalTemperature - minimalTemperature) * sinf((float)time.GetRawTime() / (24.0f * 60.0f) * M_PIF);
}
