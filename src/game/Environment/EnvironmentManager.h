#pragma once

#include "../dayTimer.h"
#include <MathLib/math.h>

// ������ ������� �����, ����, �����, ���.
// �� ������ ����� ��������� ����, ������������ �����,
// ������ �������� ������� � ������� ���� ��� ���������.

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