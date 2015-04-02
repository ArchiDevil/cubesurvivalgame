#pragma once

#if defined (WIN32) || (_WIN32)
#include <Windows.h>
#endif

class ITimer
{
public:
    virtual double GetRunningTime() const = 0;		//возвращает время, прошедшее со старта таймера
    virtual double GetDeltaTime() const = 0;		//возвращает время с последнего вызова Tick()
    virtual double GetStoppedTime() const = 0;		//время, прошедшее между остановкой и стартом

    virtual void Start() = 0;
    virtual void Tick() = 0;
    virtual void Stop() = 0;
};

#if defined (WIN32) || (_WIN32)
class cTimer : ITimer
{
public:
    cTimer();
    virtual double GetRunningTime() const;      //возвращает время, прошедшее со старта таймера
    virtual double GetDeltaTime() const;        //возвращает время с последнего вызова Tick()
    virtual double GetStoppedTime() const;      //время, прошедшее между остановкой и стартом

    virtual void Start();
    virtual void Tick();
    virtual void Stop();

private:
    long long countsPerSec; //кол-во щелчков в секунду
    double secPerCount;     //количество секунд за один щелчок

    long long startCount;   //стартовый щелчок
    long long tickCount;    //кол-во произошедших щелчков с последнего Tick()
    long long stopCount;    //щелчок, когда таймер был остановлен

    bool stopped;           //таймер остановлен?

};
#endif
