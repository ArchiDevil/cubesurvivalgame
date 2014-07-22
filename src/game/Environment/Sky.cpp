#include "Sky.h"

#include "../../GraphicsEngine/ShiftEngine.h"

using namespace ShiftEngine;

void SkyManager::Initialize()
{
	SceneGraph * pScene = GetSceneGraph();
	pScene->AddSkySceneNode();
	sunNode = pScene->AddDirectionalLightNode(-sunPos);
}

void SkyManager::Update( double /*dt*/, const Vector3F & camPos )
{
	SceneGraph * pScene = GetSceneGraph();
	pScene->GetActiveSkyNode()->GetMaterialPtr()->SetNamedParam("sunPos", sunPos);
	pScene->GetActiveSkyNode()->GetMaterialPtr()->SetNamedParam("eyePos", camPos);
	
	if(sunNode)	sunNode->SetDirection(-sunPos);
}

Vector3F SkyManager::GetSunPos() const
{
	return sunPos;
}

void SkyManager::SetSunPos( const Vector3F & pos )
{
	sunPos = pos;
}

float SkyManager::getSunIntensity() const
{
	Vector3F normSunPos = MathLib::Normalize(sunPos);
	float out = pow(MathLib::dot(Vector3F(0.0f, 0.0f, 1.0f), normSunPos) + 0.2f, 0.4f);
	return MathLib::clamp(out, 0.0f, 1.0f);
}