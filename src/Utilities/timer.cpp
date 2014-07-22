#include "timer.h"

#if defined (WIN32) || (_WIN32)

cTimer::cTimer() :
stopped(true)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	secPerCount = 1.0 / countsPerSec;
	QueryPerformanceCounter((LARGE_INTEGER*)&startCount);
	tickCount = startCount;
}

void cTimer::Start()
{
	if(stopped)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&startCount);
		stopped = false;
		this->Tick();
	}
}

void cTimer::Tick()
{
	if(!stopped)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&tickCount);
	}
}

void cTimer::Stop()
{
	if(!stopped)
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&stopCount);
		stopped = true;
	}
}

double cTimer::GetStoppedTime() const
{
	if(stopped)
	{
		long long temp;
		QueryPerformanceCounter((LARGE_INTEGER*)&temp);
		return (double)(temp - stopCount) / countsPerSec;
	}
	else
	{
		return (double)(startCount - stopCount) / countsPerSec;
	}
}

double cTimer::GetRunningTime() const
{
	if(!stopped)
	{
		long long tempCount;
		QueryPerformanceCounter((LARGE_INTEGER*)&tempCount);
		return (double)(tempCount - startCount) / countsPerSec;
	}
	return 0.0f;
}

double cTimer::GetDeltaTime() const
{
	if(!stopped)
	{
		long long tempCount;
		QueryPerformanceCounter((LARGE_INTEGER*)&tempCount);
		return (double)(tempCount - tickCount) / countsPerSec;
	}
	return 0.0f;	//lol'd
}

#endif
