#pragma once

#include "Sky.h"
#include "../dayTimer.h"

// ������ ������� �����, ����, �����, ���.
// �� ������ ����� ��������� ����, ������������ �����,
// ������ �������� ������� � ������� ���� ��� ���������.

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